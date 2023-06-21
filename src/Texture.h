#pragma once

#include <string>

#include "Color.h"

namespace MiniFPS {
    struct Texture {
        std::string name;
        Color** buffer;
        int size;

        Texture();
        Texture(const std::string& name, const std::string& filePath);
    };
}