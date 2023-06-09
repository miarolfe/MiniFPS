#pragma once

#include <SDL.h>
#include <SDL_image.h>

#include "Camera.h"
#include "Level.h"
#include "Menu.h"
#include "Texture.h"

namespace MiniFPS {
    /**
     * Sets a single pixel in the frame texture's pixel buffer.
     * @param pixels The pixel buffer of the frame texture.
     * @param pitch The pitch of the frame texture
     * @param color The color to set the pixel in the frame texture.
     * @param x The x-coordinate of the frame texture to set.
     * @param y The y-coordinate of the frame texture to set.
     */
    void SetPixel(void* pixels, int pitch, Uint32 color, int x, int y);

    /**
     * Writes the ceiling to the frame texture.
     * @param camera The camera object.
     * @param pitch The pitch of the frame texture.
     * @param pixels The pixel buffer of the frame texture.
     */
    void DrawCeiling(Camera camera, int pitch, void* pixels);

    /**
     * Writes the floor to the frame texture.
     * @param camera The camera object.
     * @param pitch The pitch of the frame texture.
     * @param pixels The pixel buffer of the frame texture.
     */
    void DrawFloor(Camera camera, int pitch, void* pixels);

    /**
     * Returns the texture buffer that corresponds to the provided texture ID.
     * @param textureId The ID of the texture for which the texture buffer is requested.
     * @param textureMap A map that maps texture IDs to texture objects.
     * @return The Texture object associated with the provided texture ID.
     */
    Texture GetTexBuffer(short textureId, std::unordered_map<short, Texture> &textureMap);

    /**
     * Renders text to a texture and copies that texture onto the frame texture.
     * @param sdlRenderer The SDL_Renderer object to use for drawing.
     * @param frameTexture The frame texture.
     * @param text The string value of the text.
     * @param font The font to use to draw the text.
     * @param x The x-coordinate to start drawing text from.
     * @param y The y-coordinate to start drawing text from.
     * @param width The required width for the drawn text.
     */
    void DrawText(SDL_Renderer* sdlRenderer, SDL_Texture* frameTexture, const std::string &text, Font font,
                  float x, float y, int width);

    /**
     * Draws the game world and copies it to the frame texture.
     * @param renderer The SDL_Renderer object to use for drawing.
     * @param player The player who's view is being drawn.
     * @param textures A map of texture IDs to textures.
     * @param streamingFrameTexture The streaming frame texture.
     * @param renderFrameTexture The render target texture.
     */
    void Draw(SDL_Renderer* renderer, Player player, std::unordered_map<short, Texture> &textures,
              SDL_Texture* streamingFrameTexture, SDL_Texture* renderFrameTexture);

    /**
     * Draws the main menu and copies it to the frame texture.
     * @param settings The settings object that holds the game's configuration.
     * @param renderer The SDL_Renderer object to use for drawing.
     * @param font The font to use for text on the menu.
     * @param camera The camera object.
     * @param streamingFrameTexture The streaming frame texture.
     * @param renderFrameTexture The render target texture.
     */
    void DrawMainMenu(const Settings &settings, SDL_Renderer* renderer, const Font &font, Camera camera,
                      SDL_Texture* streamingFrameTexture, SDL_Texture* renderFrameTexture);
}