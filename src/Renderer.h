#pragma once

#include "Camera.h"
#include "Color.h"
#include "Common.h"
#include "Enemy.h"
#include "Level.h"
#include "Menu.h"
#include "Vector.h"

namespace MiniFPS {
    const Color RED = Color(0xFFFF0000);
    const Color BLACK = Color(0xFF000000);
    const Color MAIN_MENU_BACKGROUND = Color(0xFF000000);
    const Color CEILING = Color(0xFFA5A5A5);
    const Color FLOOR = Color(0xFFBBBBDD);
    const Color BUTTON = Color(0xFFD0D0D0);
    const uint32_t TRANSPARENCY_MASK = 0xFF000000;

    struct SDLTextureBuffer {
        void* pixels = nullptr;
        int pitch = -1;
    };

    struct RaycastResult {
        bool collided = false;
        float distance = 0.0f;
        float adjustedDistance = 0.0f;
        short id = -1;
        FloatVector2 direction {0.0f, 0.0f};
        FloatVector2 sideDistance {0.0f, 0.0f};
        FloatVector2 deltaDistance {0.0f, 0.0f};
    };

    class Renderer {
    private:
        SDL_Renderer* sdlRenderer;
        SDL_Texture* streamingFrameTexture;
        SDL_Texture* renderFrameTexture;
        std::unordered_map<short, Texture> textureMap;
        float* zBuffer = nullptr;

    public:
        Renderer();
        Renderer(SDL_Renderer* sdlRenderer, const Settings& settings);

        /**
         * Set the renderer's texture map to correspond with the currently loaded level.
         * @param newTextureMap
         */
        void SetTextureMap(const std::unordered_map<short, Texture>& newTextureMap);

        /**
         * Sets a single pixel in the frame texture's pixel buffer.
         * @param buffer The frame texture buffer.
         * @param color The color to set the pixel in the frame texture.
         * @param point The pixel's coordinates in the frame texture.
         */
        void SetPixel(SDLTextureBuffer buffer, Color color, IntPoint point);

        /**
         * Checks whether to darken a pixel when rendering.
         * @param point The pixel's coordinates.
         * @return Whether to shade a pixel.
         */
        static bool ShouldShadePixel(const FloatVector2& point);

        /**
         * Returns the texture buffer that corresponds to the provided texture ID.
         * @param textureId The ID of the texture for which the texture buffer is requested.
         * @return The Texture object associated with the provided texture ID.
         */
        Texture GetTexBuffer(short textureId);

        /**
         * Returns whether a wall is west or east facing (i.e, whether the wall is "vertical").
         * @param point The point of intersection with the wall.
         * @return Whether a wall is west or east facing.
         */
        bool WallIsWestOrEastFacing(FloatPoint point);

        /**
         * Gets the appropriate texture X coordinate.
         * @param startPos The start position of the ray.
         * @param intersectionPos The position where the ray intersected a wall.
         * @param rayLength1D The one-dimensional lengths of the ray in the X and Y axes.
         * @param rayUnitStepSize The one-dimensional step sizes of the ray in the X and Y axes.
         * @param rayDir The normalized direction vector of the ray.
         * @param textureSize The length of a side of the texture in pixels (textures are always square).
         * @return The appropriate texture X coordinate.
         */
        int GetTexX(const FloatVector2& startPos, const FloatVector2& intersectionPos,
                    const FloatVector2& rayLength1D, const FloatVector2& rayUnitStepSize,
                    const FloatVector2& rayDir, int textureSize);

        /**
         * Copies a texture to the frame texture.
         * @param buffer The frame texture buffer.
         * @param texture The texture to copy to the frame texture.
         * @param point The position to copy the texture to (centered).
         * @param w The width to copy it with.
         * @param h The height to copy it with.
         */
        void CopyTextureToFrameTexture(SDLTextureBuffer buffer, const Texture& texture, IntPoint point, int w, int h);

        /**
         * Free the memory allocated to all textures.
         */
        void FreeTextures();

        /**
         * Draw a textured column to the frame texture.
         * @param texture The texture to use for the column.
         * @param camera The camera to render from.
         * @param buffer The frame texture buffer.
         * @param distance The distance from the camera to the cell.
         * @param cell The cell position the raycaster hit.
         * @param rayX The ray to draw.
         * @param texX TODO
         */
        // void DrawTexturedColumn(const Texture& texture, const Camera& camera, void* pixels, int pitch, float distance,
        //                        const FloatVector2& cell, int rayX, int texX);
        void DrawTexturedColumn(const Texture& texture, const Camera& camera, SDLTextureBuffer buffer, float distance, const FloatVector2& cell, int rayX, int texX);

        /**
         * Writes the ceiling to the frame texture.
         * @param camera The camera object.
         * @param buffer The frame texture buffer.
         */
        void DrawCeiling(const Camera& camera, SDLTextureBuffer buffer);

        /**
         * Writes the floor to the frame texture.
         * @param camera The camera object.
         * @param buffer The frame texture buffer.
         */
        void DrawFloor(const Camera& camera, SDLTextureBuffer buffer);

        /**
         * Draws a button to the frame texture.
         * @param button The button to draw.
         */
        void DrawButton(Button button);

        /**
         * Renders text to a texture and copies that texture onto the frame texture.
         * NOTE: This *can't* be called DrawText because of a conflict with a Windows API function.
         * @param text The string value of the text.
         * @param font The font to use to draw the text.
         * @param point The top-left corner of the text on the screen.
         * @param width The required width for the drawn text.
         * @param r The red value for the text.
         * @param g The green value for the text.
         * @param b The blue value for the text.
         */
        void DrawTextStr(const string &text, const Font& font, FloatPoint point, int width, int r, int g, int b);

        /**
         * Renders text to a texture and copies that texture onto the frame texture.
         * @param text The string value of the text.
         * @param font The font to use to draw the text.
         * @param point The top-left corner of the text on the screen.
         * @param height The required height for the drawn text.
         * @param r The red value for the text.
         * @param g The green value for the text.
         * @param b The blue value for the text.
         */
        void DrawTextStrH(const std::string& text, const Font& font, FloatPoint point, int height, int r, int g, int b);

        /**
         * Draws the main menu and copies it to the frame texture.
         * @param settings The settings object that holds the game's configuration.
         * @param font The font to use for text on the menu.
         * @param camera The camera object.
         */
        void DrawMainMenu(const MainMenu& mainMenu);

        /**
         * Draws enemies and copies them to the frame texture.
         * @param player The player.
         * @param enemies The enemies to draw
         * @param buffer The frame texture buffer.
         */
        // void DrawEnemies(const Player& player, const std::vector<Enemy>& enemies, void* pixels, int pitch);
        void DrawEnemies(const Player& player, const std::vector<Enemy>& enemies, SDLTextureBuffer buffer);

        /**
         * Draws the game world and copies it to the frame texture.
         * @param player The player whose view is being drawn.
         * @param enemies The enemies in the level.
         * @param font The font to use for UI.
         */
        void Draw(const Player& player, const std::vector<Enemy> &enemies, const Font& font);

        RaycastResult CastRay(int column, const Player& player);

    private:
        static bool CompareEnemyDistancePair(const std::pair<float, Enemy>& pair1, const std::pair<float, Enemy>& pair2);
    };
}