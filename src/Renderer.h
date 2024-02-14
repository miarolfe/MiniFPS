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
    const Color MAIN_MENU_BACKGROUND = Color(0xFF000000);
    const Color BUTTON = Color(0xFFD0D0D0);
    const uint32_t TRANSPARENCY_MASK = 0xFF000000;
    const short CEILING_ID = -3;
    const short FLOOR_ID = -2;

    struct SDLTextureBuffer
    {
        void* pixels = nullptr;
        int pitch = -1;
        Vec2Int size;
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
        SDL_Renderer* m_sdlRenderer;
        SDL_Texture* m_streamingFrameTexture;
        Vec2Int m_streamingFrameTextureSize;
        SDL_Texture* m_renderFrameTexture;
        Vec2Int m_renderFrameTextureSize;
        std::unordered_map<short, Texture> m_textureMap;
    public:
        float* m_zBuffer = nullptr;

    public:
        Renderer();

        Renderer(SDL_Renderer* sdlRenderer, const Settings& settings, const Vec2Int& cameraResolution);

    private:
        inline void SetPixel(const SDLTextureBuffer& buffer, const Color& color, const Vec2Int& point);

        static bool ShouldShadePixel(const Vec2& point);

        Texture GetTexBuffer(short textureId);

        bool WallIsWestOrEastFacing(const Vec2& point);

        int GetTexX(const Vec2& startPos, const RaycastResult& raycastResult, int textureSize);

        void CopyTextureToFrameTexture(const SDLTextureBuffer& buffer, const Texture& texture, const Vec2Int& point, int w, int h);

        void DrawTexturedColumn(const Texture& texture, const Camera& camera, const SDLTextureBuffer& buffer, float distance,
                                const Vec2& cell, int rayX, int texX);

        void DrawGameBackground(const Camera& camera, const SDLTextureBuffer& buffer, const Texture& floorTexture, const Texture& ceilingTexture);

        void DrawButton(const Button& button);

        void DrawTextStr(const string& text, const Font& font, const Vec2& point, int width, int r, int g, int b);

        void DrawTextStrH(const string& text, const Font& font, const Vec2& point, int height, int r, int g, int b);

        static bool CompareEnemyDistancePair(const std::pair<float, Enemy*>& pair1, const std::pair<float, Enemy*>& pair2);

        RaycastResult CastRay(int column, const Player& player);

    public:
        void FreeTextures();

        void DrawMainMenu(const MainMenu& mainMenu);

        void DrawEnemies(const Player& player, std::vector<Enemy>& enemies, const SDLTextureBuffer& buffer);

        void DrawGame(const Player& player, std::vector<Enemy>& enemies, const Font& font);

        void SetTextureMap(const std::unordered_map<short, Texture>& newTextureMap);
    };
}