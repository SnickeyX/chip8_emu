#pragma once

#include <iostream>
#include <utility>
#include <string>

namespace emulator::utils
{
    class Messenger
    {
    public:
        Messenger() = default;
        ~Messenger() = default;

        /**
         * @brief Print a message to the console
         * @param arg The first argument to print
         * @param args The rest of the arguments to print
         */
        template <typename Arg, typename... Args>
        void printMessage(Arg &&arg, Args &&...args);

        /**
         * @brief Prompt the user to enter a game to load
         * @return The name of the game to load
         */
        std::string gamePrompt();

        /**
         * @brief Print a message to the console when a game is loaded unsuccessfully
         */
        void printUnsuccessfulLoadMessage();

        /**
         * @brief Print a message to the console when the graphics library is initialised unsuccessfully
         */
        void printUnsuccessfulGraphicsInitMessage();

        /**
         * @brief Print a message to the console when the window is created unsuccessfully
         */
        void printUnsuccessfulWindowCreationMessage();

        /**
         * @brief Print a message to the console when the window is disrupted
         */
        void printUnsuccessfulDrawMessage();

        /**
         * @brief Print a message to the console when the window is closed
         */
        void printSuccessfulTerminationMessage();
    };

    template <typename Arg, typename... Args>
    void Messenger::printMessage(Arg &&arg, Args &&...args)
    {
        std::cout << std::forward<Arg>(arg);
        ((std::cout << std::forward<Args>(args)), ...);
        std::cout << "\n";
    }

} // namespace emulator::utils