#pragma once

namespace MiniFPS {
    struct Texture {
        std::string name;
        uint32_t** buffer;
        int size;

        Texture();
        Texture(const std::string& name, const std::string& filePath);
    };
}