#include "Color.h"

namespace MiniFPS
{
    Color::Color()
    {
        this->argb = 0;
    }

    Color::Color(uint32_t argb)
    {
        this->argb = argb;
    }

    Color::Color(int r, int g, int b, int a)
    {
        argb = (a << 24) + (r << 16) + (g << 8) + b;
    }

    Color Color::ShadePixel(Color pixel)
    {
        uint32_t a = pixel.argb & ALPHA_MASK;

        uint32_t r = pixel.argb & RED_MASK;
        r = r >> 1;
        r &= RED_MASK;

        uint32_t g = pixel.argb & GREEN_MASK;
        g = g >> 1;
        g &= GREEN_MASK;

        uint32_t b = pixel.argb & BLUE_MASK;
        b = b >> 1;
        b &= BLUE_MASK;

        uint32_t argbShaded = 0;
        argbShaded |= a;
        argbShaded |= r;
        argbShaded |= g;
        argbShaded |= b;

        return Color(argbShaded);
    }
}