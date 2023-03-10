#pragma once

#include "common.hpp"
#include "messages.hpp"

#include <cstdio>
#include <optional>
#include <cstring>
#include <cmath>
#include <fstream>

namespace emulator::interpreter
{
  // an emulator class for chip8
  class Chip8
  {
  public:
    Chip8(utils::Messenger &messenger);

    /**
     * @brief Load a game into memory
     * @param filename The name of the file to load
     */
    utils::Result loadGame(const char *filename);

    /**
     * @brief Emulate a single cycle of the Chip8
     * @details Fetch, decode and execute an instruction from memory[pc]
     */
    void emulateCycle();

    /**
     * @brief Get the draw flag
     * @return utils::Flag
     */
    utils::Flag shouldDraw();

    /**
     * @brief Get the terminate flag
     * @return utils::Flag
     */
    utils::Flag shouldTerminate();

    /**
     * @brief Set the key pressed by the user to the Chip8 keyboard
     * @param key The key to set
     */
    void setKey(const std::uint8_t key);

    /**
     * @brief Read a byte from the graphics buffer
     * @return the byte at the given index (optional)
     */
    std::optional<std::uint8_t> readGraphicsBuffer(const int x) const;

  private:
    /**
     * @brief Set up the Chip8 instance with default values
     * @details Set up the pc (to 0x200), memory, registers, timers, stack, keyboard and graphics buffer
     */
    void initialise();

    /**
     * @brief Decrement the delay and sound timers at 60Hz (set to 60Hz in main.cpp)
     */
    void updateTimers();

  private:
    utils::Messenger &messenger_;
    // memory model for chip8
    std::uint8_t memory[4096];

    // registers
    std::uint8_t V[16]; // 16 8-bit general purpose registers
    std::uint16_t I;    // I register to store memory addresses, only lower 12 bits are used [2^12==4k]

    // special registers
    std::uint16_t pc; // program counter
    std::uint8_t sp;  // stack pointer

    // active when nonzero, in which case subtract 1 from sound_timer at 60Hz
    // when sound_timer reaches 0, it deactivates
    // as long as value > 0, sound chip8 buzzer until it reaches 0
    std::uint8_t sound_timer;

    // timers
    // active when nonzero, in which case subtract 1 from delay_timer at 60Hz
    // when delay_timer reaches 0, it deactivates
    std::uint8_t delay_timer;

    // stack - 16 levels!
    std::uint16_t stack[16];

    // keyboard
    // only one key down during any given cycle
    // 0-15 correspond to keys 0-F
    std::uint8_t keyboard[16]; // 16 keys

    // graphics
    // 32x64 (rows x cols) pixel monochrome display - treated like a 2D array of bits
    std::uint8_t graphics_buffer[32 * 64];

    // draw flag
    utils::Flag draw;

    // terminate flag
    utils::Flag terminate;

    // To be put anywhere in the first 512 bytes of memory, where the original interpreter was located
    // I'll go with the first 80 bytes from the bottom
    std::uint8_t chip8_fontset[80] =
        {
            0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
            0x20, 0x60, 0x20, 0x20, 0x70, // 1
            0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
            0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
            0x90, 0x90, 0xF0, 0x10, 0x10, // 4
            0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
            0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
            0xF0, 0x10, 0x20, 0x40, 0x40, // 7
            0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
            0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
            0xF0, 0x90, 0xF0, 0x90, 0x90, // A
            0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
            0xF0, 0x80, 0x80, 0x80, 0xF0, // C
            0xE0, 0x90, 0x90, 0x90, 0xE0, // D
            0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
            0xF0, 0x80, 0xF0, 0x80, 0x80  // F
    };
  };
} // namespace emulator