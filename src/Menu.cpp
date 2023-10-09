#include "Menu.h"

namespace MiniFPS
{
    MainMenu::MainMenu() = default;

    MainMenu::MainMenu(const Settings &settings, const Font &font)
    {
        this->player = Player(nullptr, settings);
        this->player.inputState.inMainMenu = true;
        this->settings = settings;
        this->font = font;
        startButton = Button{{static_cast<float>(settings.screenWidth / 2),
                              static_cast<float>(3 * settings.screenHeight / 4)},
                             static_cast<float>(settings.screenWidth / 2),
                             static_cast<float>(settings.screenHeight / 6)};
    }

    void MainMenu::Update()
    {
        player.Update(0, 0, 0);
        if (startButton.Pressed(player.inputState.mousePosX, player.inputState.mousePosY) &&
            player.inputState.leftMouseButtonPressed)
        {
            player.inputState.inMainMenu = false;
        }
    }

    SDL_Texture* RenderTextToTexture(SDL_Renderer* sdlRenderer, const Font &font, const std::string &text, int r, int g,
                                     int b)
    {
        const SDL_Color textColor = {static_cast<Uint8>(r), static_cast<Uint8>(g), static_cast<Uint8>(b), 255};
        SDL_Surface* textSurface = TTF_RenderText_Solid(font.ttf, text.c_str(), textColor);
        SDL_Texture* textTexture = SDL_CreateTextureFromSurface(sdlRenderer, textSurface);
        SDL_FreeSurface(textSurface);
        return textTexture;
    }
}