#pragma once

#include "AudioHandler.h"
#include "Common.h"
#include "Enemy.h"
#include "Font.h"
#include "LogHandler.h"
#include "Settings.h"
#include "Pathfinding.h"
#include "Player.h"
#include "Renderer.h"
#include "Texture.h"
#include "Utilities.h"
#include "Vector.h"

namespace MiniFPS
{
    const float UPDATE_ENEMY_DESTINATION_TIME = 0.5f;

    enum GameState
    {
        GAME_MAIN_MENU,
        GAME_IN_GAME,
        GAME_GAME_OVER
    };

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

        void UpdateMainMenu();

        void UpdateInGame();

        void UpdateGameOver();

    private:
        GameState m_gameState = GAME_MAIN_MENU;
        SDL_Window* m_window = nullptr;
        SDL_Renderer* m_sdlRenderer = nullptr;
        FontManager m_fontManager;
        Level m_level;
        MainMenu m_mainMenu;
        Renderer m_renderer;
        Settings m_settings;
        Player m_gamePlayer;
        std::map<string, Texture> m_textureNameToTextureMap;
        std::unordered_map<short, Texture> m_textureMap;
        std::vector<Enemy> m_enemies;
        float m_oldTime = 0.0;
        float m_curTime = 0.0;
        float m_frameDelta = 0.0;
        float m_updateEnemyDestinationTimer = 0.0f;
        bool m_gameSetup = false;
        Vec2Int m_cameraResolution;
    };
}
