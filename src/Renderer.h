#pragma once

#include <SDL.h>
#include <SDL_image.h>

#include "Camera.h"
#include "Level.h"
#include "Menu.h"
#include "Texture.h"

namespace MiniFPS {
    class Renderer {
    public:
        Renderer(SDL_Renderer* sdlRenderer, Settings settings);
        SDL_Renderer* sdlRenderer;
        SDL_Texture* streamingFrameTexture;
        SDL_Texture* renderFrameTexture;
        std::unordered_map<short, Texture> textureMap;

        /**
         * Sets a single pixel in the frame texture's pixel buffer.
         * @param pixels The pixel buffer of the frame texture.
         * @param pitch The pitch of the frame texture
         * @param color The color to set the pixel in the frame texture.
         * @param x The x-coordinate of the frame texture to set.
         * @param y The y-coordinate of the frame texture to set.
         */
        void SetPixel(void* pixels, int pitch, uint32_t color, int x, int y);

        /**
         * Returns the texture buffer that corresponds to the provided texture ID.
         * @param textureId The ID of the texture for which the texture buffer is requested.
         * @return The Texture object associated with the provided texture ID.
         */
        Texture GetTexBuffer(short textureId);

        /**
         * Writes the ceiling to the frame texture.
         * @param camera The camera object.
         * @param pixels The pixel buffer of the frame texture.
         * @param pitch The pitch of the frame texture.
         */
        void DrawCeiling(Camera camera, void* pixels, int pitch);

        /**
         * Writes the floor to the frame texture.
         * @param camera The camera object.
         * @param pixels The pixel buffer of the frame texture.
         * @param pitch The pitch of the frame texture.
         */
        void DrawFloor(Camera camera, void* pixels, int pitch);

        /**
         * Renders text to a texture and copies that texture onto the frame texture.
         * @param text The string value of the text.
         * @param font The font to use to draw the text.
         * @param x The x-coordinate to start drawing text from.
         * @param y The y-coordinate to start drawing text from.
         * @param width The required width for the drawn text.
         */
        void DrawTextStr(const std::string &text, Font font, float x, float y, int width);

        /**
         * Draws the main menu and copies it to the frame texture.
         * @param settings The settings object that holds the game's configuration.
         * @param font The font to use for text on the menu.
         * @param camera The camera object.
         */
        void DrawMainMenu(const Settings &settings, const Font &font, Camera camera);

        /**
         * Draws the game world and copies it to the frame texture.
         * @param player The player who's view is being drawn.
         */
        void Draw(Player player);
    };
}