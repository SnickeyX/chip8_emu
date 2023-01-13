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

        template <typename Arg, typename... Args>
        void printMessage(Arg &&arg, Args &&...args);

        std::string gamePrompt();
        void printUnsuccessfulLoadMessage();
        void printUnsuccessfulGraphicsInitMessage();
        void printUnsuccessfulWindowCreationMessage();
        void printUnsuccessfulDrawMessage();
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