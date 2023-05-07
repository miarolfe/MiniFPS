#pragma once

#include <SDL_ttf.h>

#include "Player.h"

using string = std::string;

struct Font {
    Font();
    Font(string name, string fontPath, int pointSize);

    string name;
    TTF_Font* ttf;
    int pointSize;
};

struct MainMenu {
    explicit MainMenu(Settings settings);

    Player player;
    Settings settings;
};

SDL_Texture* RenderTextToTexture(SDL_Renderer* sdlRenderer, Font font, const std::string &text, int r, int g, int b);