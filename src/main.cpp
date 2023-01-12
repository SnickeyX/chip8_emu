#include "interpreter.hpp"
#include <chrono>
#include <thread>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
GLFWwindow *window;

int main(int argc, char **argv)
{
  emulator::Chip8 chip8;
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

  // Initialise GLFW
  if (!glfwInit())
  {
    std::cout << "Failed to initialise GLFW"
              << "\n";
    return -1;
  }

  glfwWindowHint(GLFW_SAMPLES, 4);               // 4x antialiasing
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // We want OpenGL 3.3
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  // Open a window and create its OpenGL context
  window = glfwCreateWindow(64, 32, "CHIP Display", NULL, NULL);
  if (window == NULL)
  {
    std::cout << "Failed to open GLFW window. Ensure you have the recommended libraries installed"
              << "\n";
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window);

  // Initialize GLEW
  if (glewInit() != GLEW_OK)
  {
    std::cout << "Failed to initialize GLEW"
              << "\n";
    glfwTerminate();
    return -1;
  }

  // Ensure we can capture the escape key being pressed below
  glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

  // Black background
  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

  std::chrono::system_clock::time_point time_point_start = std::chrono::system_clock::now();
  std::chrono::system_clock::time_point time_point_finish = std::chrono::system_clock::now();
  // Loop as long as we have not run of out instructions, user has not closed the window or the escape key has not been pressed
  while (chip8.shouldTerminate() == emulator::Flag::Lowered || glfwWindowShouldClose(window) == 0 || glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS)
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
      // Clear the screen. Could cause flickering otherwise.
      glClear(GL_COLOR_BUFFER_BIT);

      // Draw graphics

      glfwSwapBuffers(window);
      glfwPollEvents();
    }
    chip8.setKeys();
  }
  // Close OpenGL window and terminate GLFW
  glfwTerminate();
  std::cout << "Successfully terminated program" << std::endl;
  return 0;
}