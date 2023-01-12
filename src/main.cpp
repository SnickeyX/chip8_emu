#include "interpreter.hpp"
#include <chrono>
#include <thread>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
GLFWwindow *window;

static constexpr int MODIFIER = 10;
static constexpr int MODIFIED_WIDTH = emulator::SCREEN_WIDTH * MODIFIER;
static constexpr int MODIFIED_HEIGHT = emulator::SCREEN_HEIGHT * MODIFIER;

void DrawOnWindow(const emulator::Chip8 &C8)
{
  for (int col_num = 0; col_num < emulator::SCREEN_HEIGHT; ++col_num)
  {
    for (int row_num = 0; row_num < emulator::SCREEN_WIDTH; ++row_num)
    {
      // Setting RGB according to whether pixel is on or off
      if (C8.graphics[(col_num * emulator::SCREEN_WIDTH) + row_num] == 0)
      {
        glColor3f(0.0f, 0.0f, 0.0f);
      }
      else
      {
        glColor3f(1.0f, 1.0f, 1.0f);
      }

      glBegin(GL_QUADS);
      glVertex2f((row_num * MODIFIER), (col_num * MODIFIER));
      glVertex2f((row_num * MODIFIER), (col_num * MODIFIER) + MODIFIER);
      glVertex2f((row_num * MODIFIER) + MODIFIER, (col_num * MODIFIER) + MODIFIER);
      glVertex2f((row_num * MODIFIER) + MODIFIER, (col_num * MODIFIER));
      glEnd();
    }
  }
}

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
  glfwWindowHint(GLFW_FLOATING, GL_TRUE);

  // Open a window and create its OpenGL context
  window = glfwCreateWindow(MODIFIED_WIDTH, MODIFIED_HEIGHT, "CHIP Display", NULL, NULL);
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
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluOrtho2D(0, MODIFIED_WIDTH, MODIFIED_HEIGHT, 0);
  glMatrixMode(GL_MODELVIEW);
  glViewport(0, 0, MODIFIED_WIDTH, MODIFIED_HEIGHT);

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
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

      // Draw
      DrawOnWindow(chip8);

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