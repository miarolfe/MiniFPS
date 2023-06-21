#include <iostream>
#include <string>

#include <SDL_ttf.h>

#include "Font.h"

namespace MiniFPS {
    Font::Font() {
        ttf = nullptr;
        pointSize = -1;
    }

    Font::Font(const std::string& name, const std::string& fontPath, int pointSize) {
        this->name = name;

        TTF_Font* loadedTtf = LoadFont(fontPath, pointSize);
        if (loadedTtf != nullptr) {
            ttf = loadedTtf;
        } else {
            std::cerr << "Font " << name << " at " << fontPath << " could not be loaded" << std::endl;
        }

        this->pointSize = pointSize;
    }

    TTF_Font* Font::LoadFont(const std::string &fontPath, int pointSize) {
        return TTF_OpenFont(fontPath.c_str(), pointSize);
    }
}