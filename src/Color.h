#pragma once

#include "Common.h"

namespace MiniFPS
{
    const uint32_t RED_MASK = 0x00FF0000;
    const uint32_t GREEN_MASK = 0x0000FF00;
    const uint32_t BLUE_MASK = 0x000000FF;
    const uint32_t ALPHA_MASK = 0xFF000000;

    struct Color
    {
    public:
        Color();

        explicit Color(uint32_t argb);

        Color(int r, int g, int b, int a);

        static Color ShadePixel(Color color);

        uint32_t argb;
    };
}