#include "interpreter.hpp"

namespace emulator
{

    Chip8::Chip8()
    {
    }

    Chip8::~Chip8()
    {
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
    }

    void Chip8::loadGame(const char *filename)
    {
        std::ifstream file(filename, std::ios::binary);
        if (file.is_open())
        {
            file.seekg(0, std::ios::end);
            size_t file_size = file.tellg();
            file.seekg(0, std::ios::beg);
            char *buffer = new char[file_size];
            file.read(buffer, file_size);
            for (size_t i = 0; i < file_size; ++i)
            {
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
    }

    bool Chip8::drawFlag()
    {
        return false;
    }

    void Chip8::setKeys()
    {
    }
}