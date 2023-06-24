#pragma once

#include <SDL.h>
#include <SDL_image.h>

#include "Camera.h"
#include "Level.h"
#include "Menu.h"
#include "Texture.h"

namespace MiniFPS {
    const Color MAIN_MENU_BACKGROUND = Color(0xFF000000);
    const Color CEILING = Color(0xFFA5A5A5);
    const Color FLOOR = Color(0xFFBBBBDD);
    const Color BUTTON = Color(0xFFD0D0D0);
    const uint32_t TRANSPARENCY_MASK = 0xFF000000;

    class Renderer {
    private:
        SDL_Renderer* sdlRenderer;
        SDL_Texture* streamingFrameTexture;
        SDL_Texture* renderFrameTexture;
        std::unordered_map<short, Texture> textureMap;

    public:
        Renderer(SDL_Renderer* sdlRenderer, const Settings& settings);

        /**
         * Set the renderer's texture map to correspond with the currently loaded level.
         * @param newTextureMap
         */
        void SetTextureMap(const std::unordered_map<short, Texture>& newTextureMap);

        /**
         * Sets a single pixel in the frame texture's pixel buffer.
         * @param pixels The pixel buffer of the frame texture.
         * @param pitch The pitch of the frame texture.
         * @param color The color to set the pixel in the frame texture.
         * @param x The x-coordinate of the frame texture to set.
         * @param y The y-coordinate of the frame texture to set.
         */
        void SetPixel(void* pixels, int pitch, Color color, int x, int y);

        /**
         * Checks whether to darken a pixel when rendering.
         * @param cellX The x-coordinate of the cell.
         * @param cellY The y-coordinate of the cell.
         * @return Whether to shade a pixel.
         */
        static bool ShouldShadePixel(float cellX, float cellY);

        /**
         * Returns the texture buffer that corresponds to the provided texture ID.
         * @param textureId The ID of the texture for which the texture buffer is requested.
         * @return The Texture object associated with the provided texture ID.
         */
        Texture GetTexBuffer(short textureId);

        /**
         * Gets the appropriate texture X coordinate.
         * @param cellX The cell's X coordinate.
         * @param cellY The cell's Y coordinate.
         * @param textureSize The size of the texture.
         * @return The appropriate texture X coordinate.
         */
        int GetTexX(float cellX, float cellY, int textureSize);

        /**
         * Copies a texture to the frame texture.
         * @param pixels The pixel buffer of the frame texture.
         * @param pitch The pitch of the frame texture.
         * @param texture The texture to copy to the frame texture.
         * @param x The centered x-coordinate to copy the texture to.
         * @param y The centered y-coordinate to copy the texture to.
         * @param w The width to copy it with.
         * @param h The height to copy it with.
         */
        void CopyTextureToFrameTexture(void* pixels, int pitch, const Texture& texture, int x, int y, int w, int h);

        /**
         * Free the memory allocated to all textures.
         */
        void FreeTextures();

        void DrawTexturedColumn(const Texture& texture, Camera camera, void* pixels, int pitch, float distance,
                                float cellX, float cellY, int rayX);

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
         * Draws a button to the frame texture.
         * @param button The button to draw.
         */
        void DrawButton(Button button);

        /**
         * Renders text to a texture and copies that texture onto the frame texture.
         * NOTE: This *can't* be called DrawText because of a conflict with a Windows API function.
         * TODO: Figure out why.
         * @param text The string value of the text.
         * @param font The font to use to draw the text.
         * @param x The x-coordinate to start drawing text from.
         * @param y The y-coordinate to start drawing text from.
         * @param width The required width for the drawn text.
         * @param r The red value for the text.
         * @param g The green value for the text.
         * @param b The blue value for the text.
         */
        void DrawTextStr(const std::string &text, const Font& font, float x, float y, int width, int r, int g, int b);

        /**
         * Renders text to a texture and copies that texture onto the frame texture.
         * @param text The string value of the text.
         * @param font The font to use to draw the text.
         * @param x The x-coordinate to start drawing text from.
         * @param y The y-coordinate to start drawing text from.
         * @param height The required height for the drawn text.
         * @param r The red value for the text.
         * @param g The green value for the text.
         * @param b The blue value for the text.
         */
        void DrawTextStrH(const std::string& text, const Font& font, float x, float y, int height, int r, int g, int b);

        /**
         * Draws the main menu and copies it to the frame texture.
         * @param settings The settings object that holds the game's configuration.
         * @param font The font to use for text on the menu.
         * @param camera The camera object.
         */
        void DrawMainMenu(const MainMenu& mainMenu);

        /**
         * Draws the game world and copies it to the frame texture.
         * @param player The player who's view is being drawn.
         * @param font The font to use for UI.
         * @param frameDelta The time elapsed between frames in seconds.
         */
        void Draw(const Player& player, const Font &font, float frameDelta);
    };
}