#include "interpreter.hpp"

int main(int argc, char **argv)
{
  emulator::Chip8 chip8;
  chip8.initialise();
  chip8.loadGame("PONG");
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