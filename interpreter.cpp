#include "interpreter.hpp"

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
}

void Chip8::loadGame(const char *filename)
{
}

void Chip8::emulateCycle()
{
}

bool Chip8::drawFlag()
{
}

void Chip8::setKeys()
{
}