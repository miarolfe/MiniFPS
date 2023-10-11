#pragma once

#include "Common.h"
#include "LogHandler.h"
#include "Settings.h"

namespace MiniFPS
{
    struct Font
    {
        Font();

        Font(const string& name, const string& fontPath, int pointSize);

        string name;
        TTF_Font* ttf;
        int pointSize;

        static TTF_Font* LoadFont(const string& fontPath, int pointSize);
    };

    class FontManager
    {
    public:
        FontManager();

        explicit FontManager(const Settings& settings);

        std::vector<Font> m_fonts;

        void FreeFonts();
    };
}