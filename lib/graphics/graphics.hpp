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

        /**
         * @brief Initialise GLFW and set up hints
         */
        utils::Result initialise();

        /**
         * @brief Create a GLFW window, configure it and return it
         */
        std::optional<GLFWwindow *> getWindow();

        /**
         * @brief Draw the Chip8 screen (pixels stored in graphics_buffer) on the window
         * @param Chip8 The Chip8 interpreter to draw from
         * @param window The window to draw on
         */
        utils::Result drawOnWindow(const interpreter::Chip8 &Chip8, GLFWwindow *window);

        /**
         * @brief Check if the window has been closed or the escape key pressed
         * @param window The window to check
         */
        bool windowDisrupted(GLFWwindow *window);

        /**
         * @brief Set the key callback function for the window
         * @param Chip8 The Chip8 interpreter to pass to the callback function, so it can update the key states
         * @param window The window to set the callback function for
         */
        void setKeyReactFun(interpreter::Chip8 &Chip8, GLFWwindow *window);

    private:
        /**
         * @brief The callback function for key presses
         * @param window The window the key press occurred in
         * @param key The key that was pressed
         * @param scancode The scancode of the key
         * @param action The action that occurred (press, release, repeat)
         * @param mods The modifier keys that were pressed
         */
        void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods);

        /**
         * @brief Clear the window
         */
        void clearWindow();

    private:
        utils::Messenger messenger_;
    };

} // namespace graphics
