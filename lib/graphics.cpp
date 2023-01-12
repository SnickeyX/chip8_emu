#include "graphics.hpp"

namespace emulator::graphics
{
    Graphics::~Graphics()
    {
        glfwTerminate();
    }

    Result Graphics::initialise()
    {
        // Initialise GLFW
        if (!glfwInit())
        {
            std::cout << "Failed to initialise GLFW"
                      << "\n";
            return Result::Failure;
        }

        glfwWindowHint(GLFW_SAMPLES, 4);               // 4x antialiasing
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // We want OpenGL 3.3
        glfwWindowHint(GLFW_FLOATING, GL_TRUE);
        return Result::Success;
    }

    std::optional<GLFWwindow *> Graphics::getWindow()
    {
        // Open a window and create its OpenGL context
        GLFWwindow *window = glfwCreateWindow(MODIFIED_WIDTH, MODIFIED_HEIGHT, "CHIP Display", NULL, NULL);
        if (window == NULL)
        {
            std::cout << "Failed to open GLFW window. Ensure you have the recommended libraries installed"
                      << "\n";
            glfwTerminate();
            return std::nullopt;
        }
        glfwMakeContextCurrent(window);

        // Initialize GLEW
        if (glewInit() != GLEW_OK)
        {
            std::cout << "Failed to initialize GLEW"
                      << "\n";
            glfwTerminate();
            return std::nullopt;
        }

        // Ensure we can capture the escape key being pressed below
        glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluOrtho2D(0, MODIFIED_WIDTH, MODIFIED_HEIGHT, 0);
        glMatrixMode(GL_MODELVIEW);
        glViewport(0, 0, MODIFIED_WIDTH, MODIFIED_HEIGHT);
        return window;
    }

    Result Graphics::drawOnWindow(const interpreter::Chip8 &Chip8, GLFWwindow *window)
    {
        clearWindow();
        for (int col_num = 0; col_num < emulator::SCREEN_HEIGHT; ++col_num)
        {
            for (int row_num = 0; row_num < emulator::SCREEN_WIDTH; ++row_num)
            {
                const auto pixel_op = Chip8.readGraphicsBuffer((col_num * emulator::SCREEN_WIDTH) + row_num);
                if (!pixel_op)
                {
                    std::cout << "Failed to read graphics buffer"
                              << "\n";
                    return Result::Failure;
                }
                // Setting RGB according to whether pixel is on or off
                if (*pixel_op == 0)
                {
                    glColor3f(0.0f, 0.0f, 0.0f);
                }
                else
                {
                    glColor3f(1.0f, 1.0f, 1.0f);
                }
                // Drawing the pixel as a square
                glBegin(GL_QUADS);
                glVertex2f((row_num * MODIFIER), (col_num * MODIFIER));
                glVertex2f((row_num * MODIFIER), (col_num * MODIFIER) + MODIFIER);
                glVertex2f((row_num * MODIFIER) + MODIFIER, (col_num * MODIFIER) + MODIFIER);
                glVertex2f((row_num * MODIFIER) + MODIFIER, (col_num * MODIFIER));
                glEnd();
            }
        }
        glfwSwapBuffers(window);
        glfwPollEvents();
        return Result::Success;
    }

    bool Graphics::windowDisrupted(GLFWwindow *window)
    {
        return (glfwWindowShouldClose(window) == 0 || glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS) ? false : true;
    }

    void Graphics::keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods)
    {
    }

    void Graphics::setKeyReactFun(interpreter::Chip8 &Chip8, GLFWwindow *window)
    {
        glfwSetWindowUserPointer(window, &Chip8);
        glfwSetKeyCallback(
            window,
            [](GLFWwindow *window, int key, int scancode, int action, int mods)
            {
                auto Chip8_ptr = reinterpret_cast<interpreter::Chip8 *>(glfwGetWindowUserPointer(window));
                if (action == GLFW_PRESS || action == GLFW_REPEAT)
                {
                    switch (key)
                    {
                    case GLFW_KEY_1:
                        Chip8_ptr->setKey(0x0);
                        break;
                    case GLFW_KEY_2:
                        Chip8_ptr->setKey(0x1);
                        break;
                    case GLFW_KEY_3:
                        Chip8_ptr->setKey(0x2);
                        break;
                    case GLFW_KEY_4:
                        Chip8_ptr->setKey(0x3);
                        break;
                    case GLFW_KEY_Q:
                        Chip8_ptr->setKey(0x4);
                        break;
                    case GLFW_KEY_W:
                        Chip8_ptr->setKey(0x5);
                        break;
                    case GLFW_KEY_E:
                        Chip8_ptr->setKey(0x6);
                        break;
                    case GLFW_KEY_R:
                        Chip8_ptr->setKey(0x7);
                        break;
                    case GLFW_KEY_A:
                        Chip8_ptr->setKey(0x8);
                        break;
                    case GLFW_KEY_S:
                        Chip8_ptr->setKey(0x9);
                        break;
                    case GLFW_KEY_D:
                        Chip8_ptr->setKey(0xA);
                        break;
                    case GLFW_KEY_F:
                        Chip8_ptr->setKey(0xB);
                        break;
                    case GLFW_KEY_Z:
                        Chip8_ptr->setKey(0xC);
                        break;
                    case GLFW_KEY_X:
                        Chip8_ptr->setKey(0xD);
                        break;
                    case GLFW_KEY_C:
                        Chip8_ptr->setKey(0xE);
                        break;
                    case GLFW_KEY_V:
                        Chip8_ptr->setKey(0xF);
                        break;
                    default:
                        break;
                    }
                }
            });
    }

    void Graphics::clearWindow()
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

} // namespace emualator