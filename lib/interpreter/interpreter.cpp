#include "interpreter.hpp"

namespace emulator::interpreter
{
  Chip8::Chip8(utils::Messenger &messenger)
      : messenger_(messenger)
  {
    initialise();
  }

  void Chip8::initialise()
  {
    pc = 0x200; // program counter starts at 0x200
    I = 0;      // reset index register
    sp = 0;     // reset stack pointer

    // populate interpreter-memory with fontset
    for (size_t i = 0; i < 80; ++i)
    {
      memory[i] = chip8_fontset[i];
    }

    // clear display, keyboard and stack
    memset(graphics_buffer, 0, sizeof(graphics_buffer));
    memset(keyboard, 0, sizeof(keyboard));
    memset(stack, 0, sizeof(stack));

    delay_timer = 0;
    sound_timer = 0;

    terminate = utils::Flag::Lowered;
    draw = utils::Flag::Lowered;
  }

  // add exception handling
  utils::Result Chip8::loadGame(const char *filename)
  {
    messenger_.printMessage("Loading game ", filename, "...");
    std::ifstream file(filename, std::ios::in | std::ios::binary);
    if (file.is_open())
    {
      file.seekg(0, std::ios::end);
      const size_t file_size = file.tellg();
      file.seekg(0, std::ios::beg);
      char *buffer = new char[file_size];
      file.read(buffer, file_size);
      for (size_t i = 0; i < file_size; ++i)
      {
        // load game into memory starting at 0x200 (512)
        memory[i + 512] = static_cast<std::uint8_t>(buffer[i]);
      }
      delete[] buffer;
      messenger_.printMessage("Game loaded successfully!");
      file.close();
      return utils::Result::Success;
    }
    else
    {
      messenger_.printMessage("Failed to load game!");
      file.close();
      return utils::Result::Failure;
    }
  }

  std::optional<std::uint8_t> Chip8::readGraphicsBuffer(const int x) const
  {
    if (x < 0 || x > 2047)
    {
      return std::nullopt;
    }
    return graphics_buffer[x];
  }

  void Chip8::emulateCycle()
  {
    // opcode is 2 bytes long
    const std::uint16_t opcode = memory[pc] << 8 | memory[pc + 1];
    // all possible relevant fields from instruction
    const std::uint8_t x = (opcode & 0x0F00) >> 8;
    const std::uint8_t y = (opcode & 0x00F0) >> 4;
    const std::uint8_t n = opcode & 0x000F;
    const std::uint8_t kk = opcode & 0x00FF; // also called nn
    const std::uint16_t nnn = opcode & 0x0FFF;
    // increment program counter
    pc += 2;
    // handle opcode
    switch (opcode & 0xF000)
    {
    case 0x0000:
      switch (opcode)
      {
      // CLS - clear the display
      case 0x00E0:
        memset(graphics_buffer, 0, sizeof(graphics_buffer));
        draw = utils::Flag::Raised;
        break;
      case 0x00EE: // RET - return from subroutine
        pc = stack[--sp];
        break;
      default: // Sys addr - call RCA 1802 program at nnn - we ignore this
        break;
      }
      break;
    case 0x1000: // JP addr - jump to location nnn
      pc = nnn;
      break;
    case 0x2000: // CALL addr - call subroutine at nnn
      stack[sp] = pc;
      ++sp;
      pc = nnn;
      break;
    case 0x3000: // SE Vx,byte
      // skip next instr if Vx == kk
      pc += (V[x] == kk) ? 2 : 0;
      break;
    case 0x4000: // SNE Vx,byte
      // skip next instr if Vx != kk
      pc += (V[x] != kk) ? 2 : 0;
      break;
    case 0x5000: // SE Vx,Vy
      // skip next instr if Vx == Vy;
      pc += (V[x] == V[y]) ? 2 : 0;
      break;
    case 0x6000: // LD Vx,byte
      V[x] = kk;
      break;
    case 0x7000: // ADD Vx,byte
      V[x] += kk;
      break;
    case 0x8000:
      switch (opcode & 0x000F)
      {
      case 0x0000: // LD Vx,Vy
        V[x] = V[y];
        break;
      case 0x0001: // OR Vx,Vy
        V[x] |= V[y];
        break;
      case 0x0002: // AND Vx,Vy
        V[x] &= V[y];
        break;
      case 0x0003: // XOR Vx,Vy
        V[x] ^= V[y];
        break;
      case 0x0004: // ADD Vx,Vy
        V[0xF] = (V[y] > (0xFF - V[x])) ? 1 : 0;
        V[x] += V[y];
        break;
      case 0x0005: // SUB Vx,Vy
        V[0XF] = (V[y] > V[x]) ? 0 : 1;
        V[x] -= V[y];
        break;
      case 0x0006: // SHR Vx,Vy
        // V[x] = V[y];         // based on original implementation
        V[0XF] = V[x] & 0x1; // store least significant bit of V[x] in V[0XF]
        V[x] >>= 1;          // diviing by 2
        break;
      case 0x0007: // SUBN Vx, Vy
        V[0XF] = (V[x] > V[y]) ? 0 : 1;
        V[x] = V[y] - V[x];
        break;
      case 0x000E: // SHL Vx, {, Vy}
        // V[x] = V[y];          // based on original implementation
        V[0XF] = (V[x] >> 7); // set VF to msb of Vx
        V[x] <<= 1;           // multiply by 2
        break;
      default:
        messenger_.printMessage("Unknown opcode: ", opcode, " exiting emulator...");
        terminate = utils::Flag::Raised;
      }
      break;
    case 0x9000: // SNE Vx, Vy
      // skip next instr if Vx != Vy
      pc += (V[x] != V[y]) ? 2 : 0;
      break;
    case 0xA000: // LD I, addr
      I = nnn;
      break;
    case 0xB000:       // JP V0, addr
      pc = nnn + V[0]; // based on original implementation
      break;
    case 0xC000: // RND Vx, byte
      V[x] = floor((rand() % 256) & kk);
      break;
    case 0xD000: // DRW Vx, Vy, nibble
    {            // only starting position are wrapped around screen - based on original implementation
      std::uint8_t x_coord = V[x] % 64;
      std::uint8_t y_coord = V[y] % 32;
      V[0xF] = 0;
      for (size_t i = 0; i < n; ++i)
      {
        // stop drawing if the bottom of the scrren is reached, sprite will clip
        if (y_coord + i >= 32)
        {
          break;
        }
        std::uint8_t curr_bit_row = memory[i + I];
        for (size_t j = 0; j < 8; ++j)
        {
          // if the considered bit is set, only then do xor else there is no difference
          if (curr_bit_row & (0x80 >> j))
          {
            // stop drawing if right end of screen is reached, sprite will clip
            if (x_coord + j >= 64)
            {
              break;
            }
            // if both bits are 1 then set VF since current bit on screen is erased
            if (graphics_buffer[x_coord + j + ((y_coord + i) * 64)])
            {
              V[0XF] = 1;
            }
            graphics_buffer[x_coord + j + ((y_coord + i) * 64)] ^= 1;
          }
        }
      }
      draw = utils::Flag::Raised;
      break;
    }
    case 0xE000:
      switch (opcode & 0X00FF)
      {
      case 0X009E: // SKP Vx
        // if key corresponding to V[x] is down, skip next instr
        pc += (keyboard[V[x]] == 1) ? 2 : 0;
        break;
      case 0X00A1: // SKNP Vx
        // if key corresponding to V[x] is up, skip next instr
        pc += (keyboard[V[x]] == 0) ? 2 : 0;
        break;
      default:
        messenger_.printMessage("Unknown opcode: ", opcode, " exiting emulator...");
        terminate = utils::Flag::Raised;
      }
      break;
    case 0xF000:
      switch (opcode & 0x00FF)
      {
      case 0X0007: // LD Vx, DT
        V[x] = delay_timer;
        break;
      case 0x000A: // KD Vx, K
        // find if any key pressed, only proceeed in execution if this is the case
        for (size_t i = 0; i < 16; ++i)
        {
          if (keyboard[i])
          {
            V[x] = i;
            pc += 2;
          }
          else
          {
            // repeat the same instruction if no key is pressed
            pc -= 2;
          }
        }
        break;
      case 0x0015: // LD DT, Vx
        delay_timer = V[x];
        break;
      case 0x0018: // LD ST, Vx
        sound_timer = V[x];
        break;
      case 0x001E: // ADD I, Vx
        I += V[x];
        break;
      case 0x0029: // LD F, Vx
        // setting I to the location of the sprite for digit Vx
        // sprites stored from 0-79 with each sprite taking 5 bytes
        I = V[x] * 5;
        break;
      case 0x0033: // LD B, Vx
      {            // storing the BCD representation of the decimal value at Vx a in memory from [I:I+2]
        std::uint8_t deci = V[x];
        for (std::size_t i = 3; i-- > 0;) // prevents underflow
        {
          memory[I + i] = deci % 10;
          deci /= 10;
        }
        break;
      }
      case 0x0055: // LD [I], Vx
      {            // store values of registers Vo to Vx (inclusive) in memory starting at I, then update I
        for (size_t i = 0; i <= x; ++i)
        {
          memory[I + i] = V[i];
        }
        // I += x + 1;
        break;
      }
      case 0x0065: // LD Vx, [I]
      {            // fill registers Vo to Vx (inclusive) from memory starting at I, then update I
        for (size_t i = 0; i <= x; ++i)
        {
          V[i] = memory[I + i];
        }
        // I += x + 1;
        break;
      }
      default:
        messenger_.printMessage("Unknown opcode: ", opcode, " exiting emulator...");
        terminate = utils::Flag::Raised;
      }
      break;
    default:
      messenger_.printMessage("Unknown opcode: ", opcode, " exiting emulator...");
      terminate = utils::Flag::Raised;
      break;
    }
    updateTimers();
  }

  void Chip8::updateTimers()
  {
    if (delay_timer > 0)
    {
      --delay_timer;
    }
    if (sound_timer > 0)
    {
      messenger_.printMessage("BEEP!"); // TODO: add sound
      --sound_timer;
    }
  }

  utils::Flag Chip8::shouldDraw()
  {
    if (draw == utils::Flag::Raised)
    {
      // reset draw flag
      draw = utils::Flag::Lowered;
      return utils::Flag::Raised;
    }
    return utils::Flag::Lowered;
  }

  utils::Flag Chip8::shouldTerminate()
  {
    return terminate;
  }

  void Chip8::setKey(const std::uint8_t key)
  {
    // clear keyboard after each cycle
    memset(keyboard, 0, sizeof(keyboard));
    keyboard[key] = 1;
  }

} // namespace emulator