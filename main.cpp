#include "interpreter.hpp"

int main(int argc, char **argv)
{
    Chip8 chip8;
    chip8.initialise();
    chip8.loadGame("insert_filepath_to_rom_here");
    while (true)
    {
        chip8.emulateCycle();
        if (chip8.drawFlag())
        {
            // draw graphics
        }
        chip8.setKeys();
    }
    return 0;
}