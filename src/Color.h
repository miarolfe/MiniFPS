#pragma once

#include <cstdint>

namespace MiniFPS {
    struct Color {
    public:
        Color();
        explicit Color(uint32_t argb);
        Color(int r, int g, int b, int a);

        uint32_t argb;
    };
}