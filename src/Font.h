#pragma once

#include "Common.h"
#include "Settings.h"

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
        static TTF_Font* LoadFont(const std::string &fontPath, int pointSize);
    };

    class FontManager {
    public:
        FontManager();
        explicit FontManager(const Settings& settings);

        std::vector<Font> fonts;

        void FreeFonts();
    };
}