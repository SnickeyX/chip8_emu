#include "messages.hpp"

namespace emulator::utils
{

    std::string Messenger::gamePrompt()
    {
        std::cout << "Welcome to my Chip8 emulator!"
                  << "\n"
                  << "Which game (or file) would you like to use today?"
                  << "\n";
        std::cout << "Enter the name of the desired file below (ensure file is present in the files folder!)"
                  << "\n";
        std::string filename;
        std::cin >> filename;
        return filename;
    }

    void Messenger::printUnsuccessfulLoadMessage()
    {
        std::cout << "Error: Exiting program due to unsuccessful file load"
                  << "\n"
                  << "Ensure file name is correct and file is present in the files folder!"
                  << "\n";
    }

    void Messenger::printUnsuccessfulGraphicsInitMessage()
    {
        std::cout << "Error: Exiting program due to unsuccessful graphics initialisation"
                  << "\n";
    }

    void Messenger::printUnsuccessfulWindowCreationMessage()
    {
        std::cout << "Error: Exiting program due to unsuccessful window creation"
                  << "\n";
    }

    void Messenger::printUnsuccessfulDrawMessage()
    {
        std::cout << "Error: Exiting program due to unsuccessful window draw"
                  << "\n";
    }

    void Messenger::printSuccessfulTerminationMessage()
    {
        std::cout << "Successfully terminated program"
                  << "\n";
    }

} // namespace emulator::utils