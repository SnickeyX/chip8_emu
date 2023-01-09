#include "interpreter.hpp"

int main(int argc, char **argv)
{
  emulator::Chip8 chip8;
  chip8.initialise();
  std::cout << "Welcome to my Chip8 emulator!"
            << "\n"
            << "Which game (or file) would you like to use today?"
            << "\n";
  std::cout << "Enter the name of the desired file below (ensure file is present in the files folder!)"
            << "\n";
  std::string filename;
  std::cin >> filename;
  bool successfull_load = chip8.loadGame(filename.c_str());
  if (!successfull_load)
  {
    std::cout << "Error: Exiting program due to unsuccessful file load"
              << "\n"
              << "Ensure file name is correct and file is present in the files folder!"
              << "\n";
    return 1;
  }
  while (chip8.shouldTerminate() == emulator::Flag::Lowered)
  {
    chip8.emulateCycle();
    if (chip8.shouldDraw() == emulator::Flag::Raised)
    {
      // draw graphics
    }
    chip8.setKeys();
  }
  std::cout << "Successfully terminated program" << std::endl;
  return 0;
}