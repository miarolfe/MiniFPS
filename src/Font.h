#pragma once

#include <string>

#include <SDL_ttf.h>

namespace MiniFPS {
    struct Font {
        Font();
        Font(const std::string &name, const std::string &fontPath, int pointSize);

        std::string name;
        TTF_Font* ttf;
        int pointSize;

        /**
         * Loads a font from a file.
         * @param fontPath The path to the font file.
         * @param pointSize The point size to load the font at.
         * @return The loaded font.
         */
        TTF_Font* LoadFont(const std::string &fontPath, int pointSize);
    };
}