#pragma once

#include <SDL_ttf.h>

#include "Player.h"

using string = std::string;

namespace MiniFPS {
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
    SDL_Texture* RenderTextToTexture(SDL_Renderer* sdlRenderer, Font font, const std::string &text, int r, int g, int b);
}