#pragma once
#include "Color.h"
struct Hold
{
    Color color;
    int index;
    /// @brief Constructor initialises color and index
    /// @param color the color of the hold
    /// @param index the index of the hold (moonboard style index)
    Hold(Color color, int index) : color(color), index(index){}
    Hold() = default;
};