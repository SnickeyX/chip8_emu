#pragma once

#include <cstdint>
#include <iostream>

namespace emulator
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
}