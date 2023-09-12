#pragma once

#include <unordered_map>
#include "SDL.h"

#include "Audio.h"
#include "Font.h"
#include "Settings.h"
#include "Renderer.h"
#include "Texture.h"

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
        Settings settings;
        Audio audio;
        FontManager fontManager;
        std::map<std::string, Texture> textureNameToTextureMap;
        Renderer renderer;
        MainMenu mainMenu;
        Level level;
        std::unordered_map<short, Texture> textureMap;
        Player gamePlayer;
        float oldTime = 0.0;
        float curTime = 0.0;
        float frameDelta = 0.0;
        bool gameSetup = false;
    };
}
