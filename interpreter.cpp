#include "interpreter.hpp"

namespace emulator
{
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

    // clear display and keyboard
    memset(graphics, 0, sizeof(graphics));
    memset(keyboard, 0, sizeof(keyboard));

    delay_timer = 0;
    sound_timer = 0;
  }

  // add exception handling
  void Chip8::loadGame(const char *filename)
  {
    std::ifstream file(filename, std::ios::binary);
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
    }
    else
    {
      std::cout << "Error: Could not open file" << std::endl;
    }
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
    // handle opcode
    switch (opcode & 0xF000)
    {
    case 0x0000:
      switch (opcode & 0x000F)
      {
      // CLS - clear the display
      case 0x0000:
        memset(graphics, 0, sizeof(graphics));
        pc += 2;
        break;
      case 0x000E: // RET - return from subroutine
        pc = stack[sp];
        --sp;
        break;
      }
      break;
    case 0x1000: // JP addr - jump to location nnn
      pc = opcode & 0x0FFF;
      break;
    case 0x2000: // CALL addr - call subroutine at nnn
      ++sp;
      stack[sp] = pc;
      pc = opcode & 0x0FFF;
      break;
    case 0x3000: // SE Vx,byte
      // skip next instr if Vx == kk
      pc += (V[x] == kk) ? 4 : 2;
      break;
    case 0x4000: // SNE Vx,byte
      // skip next instr if Vx != kk
      pc += (V[x] != kk) ? 4 : 2;
      break;
    case 0x5000: // SE Vx,Vy
      // skip next instr if Vx == Vy;
      pc += (V[x] == V[y]) ? 4 : 2;
      break;
    case 0x6000: // LD Vx,byte
      V[x] = kk;
      pc += 2;
      break;
    case 0x7000: // ADD Vx,byte
      V[x] += kk;
      pc += 2;
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
      case 0x0006:           // SHR Vx,Vy
        V[0XF] = V[x] & 0x1; // store least significant bit of V[x] in V[0XF]
        V[x] >>= 1;          // diviing by 2
        break;
      case 0x0007: // SUBN Vx, Vy
        V[0XF] = (V[x] > V[y]) ? 0 : 1;
        V[x] = V[y] - V[x];
        break;
      case 0x000E:            // SHL Vx, {, Vy}
        V[0XF] = (V[x] >> 7); // set VF to msb of Vx
        V[x] <<= 2;           // multiply by 2
        break;
      }
      pc += 2;
      break;
    case 0x9000: // SNE Vx, Vy
      // skip next instr if Vx != Vy
      pc += (V[x] != V[y]) ? 4 : 2;
      break;
    case 0xA000: // LD I, addr
      I = nnn;
      pc += 2;
      break;
    case 0xB000: // JP V0, addr
      pc = nnn + V[0];
      break;
    case 0xC000: // RND Vx, byte
      V[x] = (rand() % 256) & kk;
      pc += 2;
      break;
    case 0xD000: // DRW Vx, Vy, nibble
      for (size_t i = 0; i < n; ++i)
      {
        const std::uint8_t curr_bit_row = memory[i + I];
        // TODO
      }
      pc += 2;
      break;
    case 0xE000:
      switch (opcode & 0X000F)
      {
      case 0X000E: // SKP Vx
        // if key corresponding to V[x] is down, skip next instr
        pc += (keyboard[V[x]]) ? 4 : 2;
        break;
      case 0X0001: // SKNP Vx
        // if key corresponding to V[x] is up, skip next instr
        pc += (!keyboard[V[x]]) ? 4 : 2;
        break;
      }
      break;
    case 0xF000:
      switch (opcode & 0x00FF)
      {
      case 0X0007: // LD Vx, DT
        V[x] = delay_timer;
        pc += 2;
        break;
      case 0x000A: // KD Vx, K
        // find if any key pressed, only proceeed in execution if this is the case
        for (std::int8_t i = 0; i < 16; ++i)
        {
          if (keyboard[i])
          {
            V[x] = i;
            pc += 2;
          }
        }
        break;
      case 0x0015: // LD DT, Vx
        delay_timer = V[x];
        pc += 2;
        break;
      case 0x0018: // LD ST, Vx
        sound_timer = V[x];
        pc += 2;
        break;
      case 0x001E: // ADD I, Vx
        I += V[x];
        pc += 2;
        break;
      case 0x0029: // LD F, Vx
        I = V[x] * 5;
        pc += 2;
        break;
      case 0x0033: // LD B, Vx
        // storing the BCD representation of the decimal value at Vx a in memory from [I:I+2]
        std::uint8_t deci = V[x];
        for (std::uint8_t i = 2; i >= 0; --i)
        {
          memory[I + i] = deci % 10;
          deci /= 10;
        }
        pc += 2;
        break;
      case 0x0055: // LD [I], Vx
        // store values of registers Vo to Vx (inclusive) in memory starting at I, then update I
        for (std::uint8_t i = 0; i <= x; ++i)
        {
          memory[I + i] = V[i];
        }
        I += x + 1;
        pc += 2;
        break;
      case 0x0065: // LD Vx, [I]
        // fill registers Vo to Vx (inclusive) from memory starting at I, then update I
        for (std::uint8_t i = 0; i <= x; ++i)
        {
          V[i] = memory[I + i];
        }
        I += x + 1;
        pc += 2;
        break;
      }
      break;
    }
  }

  bool Chip8::drawFlag()
  {
    return false;
  }

  void Chip8::setKeys()
  {
  }
}