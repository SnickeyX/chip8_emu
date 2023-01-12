#include "interpreter.hpp"
#include "graphics.hpp"

#include <chrono>
#include <thread>

int main(int argc, char **argv)
{
  emulator::interpreter::Chip8 chip8;
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
  };
  emulator::graphics::Graphics graphics_handler;
  const auto graphics_init_result = graphics_handler.initialise();
  if (graphics_init_result == emulator::Result::Failure)
  {
    std::cout << "Error: Exiting program due to unsuccessful graphics initialisation"
              << "\n";
    return 1;
  }
  const auto window_op = graphics_handler.getWindow();
  if (!window_op)
  {
    std::cout << "Error: Exiting program due to unsuccessful window creation"
              << "\n";
    return 1;
  }
  graphics_handler.setKeyReactFun(chip8, window_op.value());
  std::chrono::system_clock::time_point time_point_start = std::chrono::system_clock::now();
  std::chrono::system_clock::time_point time_point_finish = std::chrono::system_clock::now();
  // Loop as long as we have not run of out instructions, user has not closed the window or the escape key has not been pressed
  while (chip8.shouldTerminate() == emulator::Flag::Lowered || graphics_handler.windowDisrupted(window_op.value()))
  {
    // Maintain designated frequency of 60 Hz (roughly 16.6 ms per frame)
    time_point_start = std::chrono::system_clock::now();
    std::chrono::duration<double, std::milli> work_time = time_point_start - time_point_finish;
    if (work_time.count() < 16.0) // leaving a little time for these calculations to be done
    {
      std::chrono::duration<double, std::milli> delta_ms(16.0 - work_time.count());
      const auto delta_ms_duration = std::chrono::duration_cast<std::chrono::milliseconds>(delta_ms);
      std::this_thread::sleep_for(std::chrono::milliseconds(delta_ms_duration.count()));
    }
    time_point_finish = std::chrono::system_clock::now();
    chip8.emulateCycle();
    if (chip8.shouldDraw() == emulator::Flag::Raised)
    {
      // updating window with new graphics
      const auto draw_result = graphics_handler.drawOnWindow(chip8, window_op.value());
      if (draw_result == emulator::Result::Failure)
      {
        std::cout << "Error: Exiting program due to unsuccessful window draw"
                  << "\n";
        return 1;
      }
    }
  }
  std::cout << "Successfully terminated program" << std::endl;
  return 0;
}