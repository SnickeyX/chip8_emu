#pragma once

#include "common.hpp"
#include "interpreter.hpp"

#include <optional>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

namespace emulator::graphics
{
    static constexpr int MODIFIER = 10;
    static constexpr int MODIFIED_WIDTH = SCREEN_WIDTH * MODIFIER;
    static constexpr int MODIFIED_HEIGHT = SCREEN_HEIGHT * MODIFIER;

    class Graphics
    {
    public:
        Graphics() = default;
        ~Graphics();

        Result initialise();
        std::optional<GLFWwindow *> getWindow();
        Result drawOnWindow(const interpreter::Chip8 &Chip8, GLFWwindow *window);
        bool windowDisrupted(GLFWwindow *window);
        void clearWindow();
    };

} // namespace graphics
