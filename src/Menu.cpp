#include "Menu.h"

namespace MiniFPS
{
    MainMenu::MainMenu() = default;

    MainMenu::MainMenu(const Settings& settings, const Font& font, const Vec2Int& cameraResolution)
    {
        this->m_player = Player(nullptr, settings, cameraResolution);
        this->m_player.m_inputState.inMainMenu = true;
        this->m_settings = settings;
        this->m_font = font;
        m_startButton = Button{{static_cast<float>(cameraResolution.x / 2),
                                static_cast<float>(3 * cameraResolution.y / 4)},
                               {static_cast<float>(cameraResolution.x / 2),
                               static_cast<float>(cameraResolution.y / 6)}};
    }


    void MainMenu::Update(const Vec2Int& displayResolution, const Vec2Int& cameraResolution)
    {
        m_player.Update(0, 0, 0);

        const Vec2 mouseScalingFactor
        {
            static_cast<float>(cameraResolution.x) / static_cast<float>(displayResolution.x),
            static_cast<float>(cameraResolution.y) / static_cast<float>(displayResolution.y)
        };

        const Vec2Int scaledMousePos
        {
            static_cast<int>(static_cast<float>(m_player.m_inputState.mousePosX) * mouseScalingFactor.x),
            static_cast<int>(static_cast<float>(m_player.m_inputState.mousePosY) * mouseScalingFactor.y)
        };

        if (m_startButton.Pressed(scaledMousePos) &&
            m_player.m_inputState.leftMouseButtonPressed)
        {
            m_player.m_inputState.inMainMenu = false;
        }
    }

    SDL_Texture* RenderTextToTexture(SDL_Renderer* sdlRenderer, const Font& font, const string& text, int r, int g,
                                     int b)
    {
        const SDL_Color textColor = {static_cast<Uint8>(r), static_cast<Uint8>(g), static_cast<Uint8>(b), 255};
        SDL_Surface* textSurface = TTF_RenderText_Solid(font.ttf, text.c_str(), textColor);
        SDL_Texture* textTexture = SDL_CreateTextureFromSurface(sdlRenderer, textSurface);
        SDL_FreeSurface(textSurface);
        return textTexture;
    }
}