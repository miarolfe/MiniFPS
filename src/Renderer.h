#pragma once

#include "Camera.h"
#include "Color.h"
#include "Common.h"
#include "Enemy.h"
#include "Level.h"
#include "Menu.h"
#include "Vector.h"

namespace MiniFPS
{
    const Color RED = Color(0xFFFF0000);
    const Color BLACK = Color(0xFF000000);
    const Color MAIN_MENU_BACKGROUND = Color(0xFF000000);
    const Color CEILING = Color(0xFFA5A5A5);
    const Color FLOOR = Color(0xFFBBBBDD);
    const Color BUTTON = Color(0xFFD0D0D0);
    const uint32_t TRANSPARENCY_MASK = 0xFF000000;

    struct SDLTextureBuffer
    {
        void* pixels = nullptr;
        int pitch = -1;
    };

    struct RaycastResult
    {
        bool collided = false;
        float distance = 0.0f;
        float adjustedDistance = 0.0f;
        short id = -1;
        Vec2 direction{0.0f, 0.0f};
        Vec2 sideDistance{0.0f, 0.0f};
        Vec2 deltaDistance{0.0f, 0.0f};
    };

    class Renderer
    {
    private:
        SDL_Renderer* sdlRenderer;
        SDL_Texture* streamingFrameTexture;
        SDL_Texture* renderFrameTexture;
        std::unordered_map<short, Texture> textureMap;
    public:
        float* zBuffer = nullptr;

    public:
        Renderer();

        Renderer(SDL_Renderer* sdlRenderer, const Settings &settings);

    private:
        void SetPixel(SDLTextureBuffer buffer, Color color, IntPoint point);

        static bool ShouldShadePixel(const Vec2 &point);

        Texture GetTexBuffer(short textureId);

        bool WallIsWestOrEastFacing(FloatPoint point);

        int
        GetTexX(const Vec2 &startPos, const Vec2 &intersectionPos, const Vec2 &rayLength1D, const Vec2 &rayUnitStepSize,
                const Vec2 &rayDir, int textureSize);

        void CopyTextureToFrameTexture(SDLTextureBuffer buffer, const Texture &texture, IntPoint point, int w, int h);

        void DrawTexturedColumn(const Texture &texture, const Camera &camera, SDLTextureBuffer buffer, float distance,
                                const Vec2 &cell, int rayX, int texX);

        void DrawCeiling(const Camera &camera, SDLTextureBuffer buffer);

        void DrawFloor(const Camera &camera, SDLTextureBuffer buffer);

        void DrawButton(Button button);

        void DrawTextStr(const string &text, const Font &font, FloatPoint point, int width, int r, int g, int b);

        void DrawTextStrH(const std::string &text, const Font &font, FloatPoint point, int height, int r, int g, int b);

    public:
        RaycastResult CastRay(int column, const Player &player);

        void FreeTextures();

        void DrawMainMenu(const MainMenu &mainMenu);

        void DrawEnemies(const Player &player, std::vector<Enemy> &enemies, SDLTextureBuffer buffer);

        void DrawGame(const Player &player, std::vector<Enemy> &enemies, const Font &font);

        void SetTextureMap(const std::unordered_map<short, Texture> &newTextureMap);

    private:
        static bool
        CompareEnemyDistancePair(const std::pair<float, Enemy> &pair1, const std::pair<float, Enemy> &pair2);
    };
}