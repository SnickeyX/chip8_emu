#pragma once

#include "common.hpp"
#include "messages.hpp"
#include "interpreter.hpp"

#include <optional>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

namespace emulator::graphics
{
    static constexpr int MODIFIER = 10;
    static constexpr int MODIFIED_WIDTH = utils::SCREEN_WIDTH * MODIFIER;
    static constexpr int MODIFIED_HEIGHT = utils::SCREEN_HEIGHT * MODIFIER;

    class Graphics
    {
    public:
        Graphics(utils::Messenger &messenger);
        ~Graphics();

        utils::Result initialise();
        std::optional<GLFWwindow *> getWindow();
        utils::Result drawOnWindow(const interpreter::Chip8 &Chip8, GLFWwindow *window);
        bool windowDisrupted(GLFWwindow *window);
        void setKeyReactFun(interpreter::Chip8 &Chip8, GLFWwindow *window);

    private:
        void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods);
        void clearWindow();

    private:
        utils::Messenger messenger_;
    };

} // namespace graphics