#pragma once

#include "Button.h"
#include "Camera.h"
#include "Common.h"
#include "Font.h"
#include "Player.h"

namespace MiniFPS
{
    class MainMenu
    {
    public:
        MainMenu();

        MainMenu(const Settings& settings, const Font& font);

        Font m_font;
        Player m_player;
        Settings m_settings;
        Button m_startButton;

        void Update(const Vec2Int& displayResolution);
    };

    SDL_Texture*
    RenderTextToTexture(SDL_Renderer* sdlRenderer, const Font& font, const string& text, int r, int g, int b);
}