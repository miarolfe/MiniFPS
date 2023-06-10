#pragma once

#include <SDL.h>
#include <SDL_image.h>

#include "Camera.h"
#include "Level.h"
#include "Menu.h"

namespace MiniFPS {
    struct Texture {
        string name;
        uint32_t** buffer;
        int size;

        Texture();
        Texture(const std::string& name, const std::string& filePath);
    };
}