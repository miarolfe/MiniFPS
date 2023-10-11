#include "Font.h"
#include "Utilities.h"

namespace MiniFPS
{
    Font::Font()
    {
        ttf = nullptr;
        pointSize = -1;
    }

    Font::Font(const string& name, const string& fontPath, int pointSize)
    {
        this->name = name;

        TTF_Font* loadedTtf = LoadFont(fontPath, pointSize);
        if (loadedTtf != nullptr)
        {
            ttf = loadedTtf;
        }
        else
        {
            string warning;
            warning += "Failed to load font ";
            warning += name;
            warning += " at ";
            warning += fontPath;
            LogHandler::GetInstance().LogWarning(warning.c_str());
        }

        this->pointSize = pointSize;
    }

    TTF_Font* Font::LoadFont(const string& fontPath, int pointSize)
    {
        return TTF_OpenFont(fontPath.c_str(), pointSize);
    }

    FontManager::FontManager()
    {

    }

    FontManager::FontManager(const Settings& settings)
    {
        for (const auto& fontPath: settings.fontPaths)
        {
            m_fonts.emplace_back(fontPath.first, GetSDLAssetsFolderPath() + fontPath.second, 24);
        }
    }

    void FontManager::FreeFonts()
    {
        for (const Font& font: m_fonts)
        {
            TTF_CloseFont(font.ttf);
        }
    }
}