#pragma once

#include <SDL_ttf.h>

#include "Button.h"
#include "Font.h"
#include "Player.h"

using string = std::string;

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

    /**
     * Renders a text string to a texture.
     * @param sdlRenderer The SDL_Renderer object to use for drawing.
     * @param font The font to use to draw the text.
     * @param text The string value of the text.
     * @param r The red value to tint the text with.
     * @param g The green value to tint the text with.
     * @param b The blue value to tint the text with.
     * @return A texture showing the rendered text string.
     */
    SDL_Texture* RenderTextToTexture(SDL_Renderer* sdlRenderer, const Font& font, const std::string &text, int r, int g, int b);
}