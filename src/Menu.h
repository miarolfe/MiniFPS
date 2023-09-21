#pragma once

#include "Button.h"
#include "Common.h"
#include "Font.h"
#include "Player.h"

namespace MiniFPS {
    class MainMenu {
    public:
        MainMenu();
        MainMenu(const Settings& settings, const Font& font);

        Font font;
        Player player;
        Settings settings;
        Button startButton;

        void Update();
    };

    SDL_Texture* RenderTextToTexture(SDL_Renderer* sdlRenderer, const Font& font, const std::string &text, int r, int g, int b);
}