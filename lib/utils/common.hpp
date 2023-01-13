#pragma once

#include <cstdint>

namespace emulator::utils
{
    static constexpr int SCREEN_WIDTH = 64;
    static constexpr int SCREEN_HEIGHT = 32;

    enum class Flag
    {
        Raised,
        Lowered
    };

    enum class Result
    {
        Success,
        Failure
    };
} // namespace emulator::utils