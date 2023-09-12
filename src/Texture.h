#pragma once

#include "Color.h"
#include "Common.h"

namespace MiniFPS {
    struct Texture {
        std::string name;
        Color** buffer;
        int size;

        Texture();
        Texture(const std::string& name, const std::string& filePath);
    };
}