#pragma once

#include "Audio.h"
#include "Common.h"
#include "Enemy.h"
#include "Font.h"
#include "Settings.h"
#include "Renderer.h"
#include "Texture.h"
#include "Utilities.h"

namespace MiniFPS
{
    class Game
    {
    public:
        Game();

        void Update();

        bool IsRunning();

        ~Game();

    private:
        void LoadTextures();

        void SetupGame();

    private:
        SDL_Window* m_window = nullptr;
        SDL_Renderer* m_sdlRenderer = nullptr;
        Audio m_audio;
        FontManager m_fontManager;
        Level m_level;
        MainMenu m_mainMenu;
        Renderer m_renderer;
        Settings m_settings;
        Player m_gamePlayer;
        std::map<std::string, Texture> m_textureNameToTextureMap;
        std::unordered_map<short, Texture> m_textureMap;
        std::vector<Enemy> m_enemies;
        float m_oldTime = 0.0;
        float m_curTime = 0.0;
        float m_frameDelta = 0.0;
        bool m_gameSetup = false;
    };
}
