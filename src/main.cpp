#include "interpreter.hpp"
#include "messages.hpp"
#include "graphics.hpp"

#include <chrono>
#include <thread>

void updateTimePoints(std::chrono::system_clock::time_point &time_point_start, std::chrono::system_clock::time_point &time_point_finish);

int main()
{
  // create a messenger to print messages to the user
  emulator::utils::Messenger messenger;
  const std::string filename = messenger.gamePrompt();
  // create a chip8 instance and load the game
  emulator::interpreter::Chip8 chip8(messenger);
  const auto game_load_result = chip8.loadGame(filename.c_str());
  if (game_load_result == emulator::utils::Result::Failure)
  {
    messenger.printUnsuccessfulLoadMessage();
    return 1;
  };
  // create a graphics handler and initialise the graphics library
  emulator::graphics::Graphics graphics_handler(messenger);
  const auto graphics_init_result = graphics_handler.initialise();
  if (graphics_init_result == emulator::utils::Result::Failure)
  {
    messenger.printUnsuccessfulGraphicsInitMessage();
    return 1;
  }
  const auto window_op = graphics_handler.getWindow();
  if (!window_op)
  {
    messenger.printUnsuccessfulWindowCreationMessage();
    return 1;
  }
  graphics_handler.setKeyReactFun(chip8, window_op.value());
  // create time points to maintain 60 Hz
  std::chrono::system_clock::time_point time_point_start = std::chrono::system_clock::now();
  std::chrono::system_clock::time_point time_point_finish = std::chrono::system_clock::now();
  // Loop as long as we have not run of out instructions, user has not closed the window or the escape key has not been pressed
  while (chip8.shouldTerminate() == emulator::utils::Flag::Lowered || graphics_handler.windowDisrupted(window_op.value()))
  {
    updateTimePoints(time_point_start, time_point_finish);
    chip8.emulateCycle();
    if (chip8.shouldDraw() == emulator::utils::Flag::Raised)
    {
      // updating window with new graphics
      const auto draw_result = graphics_handler.drawOnWindow(chip8, window_op.value());
      if (draw_result == emulator::utils::Result::Failure)
      {
        messenger.printUnsuccessfulDrawMessage();
        return 1;
      }
    }
  }
  messenger.printSuccessfulTerminationMessage();
  return 0;
}

// sleep the main thread to maintain 60 Hz
void updateTimePoints(std::chrono::system_clock::time_point &time_point_start, std::chrono::system_clock::time_point &time_point_finish)
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
}