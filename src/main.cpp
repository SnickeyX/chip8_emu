#include "interpreter.hpp"

int main(int argc, char **argv)
{
  emulator::Chip8 chip8;
  chip8.initialise();
  chip8.loadGame("PONG");
  for (;;)
  {
    chip8.emulateCycle();
    if (chip8.drawFlag())
    {
      // draw graphics
    }
    chip8.setKeys();
  }
  std::cout << "Successfully competed program" << std::endl;
  return 0;
}