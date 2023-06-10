#include <iostream>

#include "Menu.h"
#include "Utilities.h"

namespace MiniFPS {
    Font::Font() {
        ttf = nullptr;
        pointSize = -1;
    }

    Font::Font(const string& name, const string& fontPath, int pointSize) {
        this->name = name;

        TTF_Font* loadedTtf = LoadFont(fontPath, pointSize);
        if (loadedTtf != nullptr) {
            ttf = loadedTtf;
        } else {
            std::cerr << "Font " << name << " at " << fontPath << " could not be loaded" << std::endl;
        }

        this->pointSize = pointSize;
    }

    MainMenu::MainMenu(const Settings& settings) {
        this->player = Player(nullptr, settings);
        this->player.inputState.inMainMenu = true;
        this->settings = settings;
    }

    SDL_Texture* RenderTextToTexture(SDL_Renderer* sdlRenderer, const Font& font, const std::string &text, int r, int g,
                                     int b) {
        SDL_Color textColor = {static_cast<Uint8>(r), static_cast<Uint8>(g), static_cast<Uint8>(b), 255};
        SDL_Surface* textSurface = TTF_RenderText_Solid(font.ttf, text.c_str(), textColor);
        SDL_Texture* textTexture = SDL_CreateTextureFromSurface(sdlRenderer, textSurface);
        SDL_FreeSurface(textSurface);
        return textTexture;
    }
}