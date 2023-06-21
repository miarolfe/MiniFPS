#include <cstdint>

#include "Color.h"

namespace MiniFPS {
    Color::Color() {
        this->argb = 0;
    }

    Color::Color(uint32_t argb) {
        this->argb = argb;
    }

    Color::Color(int r, int g, int b, int a) {
        argb = (a << 24) + (r << 16) + (g << 8) + b;
    }
}