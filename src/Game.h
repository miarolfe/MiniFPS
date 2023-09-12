#pragma once

#include "Audio.h"
#include "Common.h"
#include "Enemy.h"
#include "Font.h"
#include "Settings.h"
#include "Renderer.h"
#include "Texture.h"
#include "Utilities.h"

namespace MiniFPS {
    class Game {
    public:
        Game();
        void Update();
        bool IsRunning();
        ~Game();

    private:
        void LoadTextures();
        void SetupGame();

    private:
        SDL_Window* window = nullptr;
        SDL_Renderer* sdlRenderer = nullptr;
        Audio audio;
        FontManager fontManager;
        Level level;
        MainMenu mainMenu;
        Renderer renderer;
        Settings settings;
        Player gamePlayer;
        std::map<std::string, Texture> textureNameToTextureMap;
        std::unordered_map<short, Texture> textureMap;
        std::vector<Enemy> enemies;
        float oldTime = 0.0;
        float curTime = 0.0;
        float frameDelta = 0.0;
        bool gameSetup = false;
    };
}
