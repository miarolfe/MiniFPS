#define SDL_MAIN_HANDLED

#include <iostream>
#include <vector>

#include <SDL.h>

#include "Color.h"
#include "Level.h"
#include "Player.h"
#include "Renderer.h"
#include "Settings.h"
#include "Utilities.h"

int main() {
    ClearFile("log.txt");

    if (!InitializeSDL()) {
        std::cerr << "SDL could not be initialized:" << SDL_GetError();
    } else {
        std::cout << "SDL initialized" << std::endl;
    }

    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    const std::string assetsFolderPath = GetSDLAssetsFolderPath();
    const std::string settingsPath = "settings.json";

    Settings settings = Settings::LoadSettings(assetsFolderPath, settingsPath);

    if (!InitializeWindowAndRenderer(&window, &renderer, settings.screenWidth, settings.screenHeight, settings.vSync)) {
        std::cerr << "Window and/or renderer could not be initialized" << std::endl;
    } else {
        std::cout << "Window and renderer initialized" << std::endl;
    }

    if (!InitializeSDLImage()) {
        std::cerr << "SDL_image could not be initialized" << std::endl;
    } else {
        std::cout << "SDL_image initialized" << std::endl;
    }

    size_t wallTexSize;
    // TODO: Allow variable size textures
    // Right now all wall textures must be the same size
    size_t numWallTextures = settings.texturePaths.size();
    Uint32*** wallTextureBuffers = new Uint32** [numWallTextures];
    for (size_t buffer = 0; buffer < numWallTextures; buffer++) {
        LoadTextureToBuffer(&wallTextureBuffers[buffer], wallTexSize, assetsFolderPath,
                            settings.texturePaths[buffer]);
    }

    Level level = Level(GetSDLAssetsFolderPath() + settings.levelPath);
    level.Print();

    Player player(&level, settings);

    bool started = false;

    float oldTime, curTime, frameDelta;
    curTime = 0;

    SDL_Texture* frameTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING,
                                                  static_cast<int>(player.camera.viewportWidth), static_cast<int>(player.camera.viewportHeight));

    while (!player.GameHasEnded()) {
        oldTime = curTime;
        curTime = static_cast<float>(SDL_GetTicks64());

        frameDelta = GetFrameTime(oldTime, curTime);

        player.Update(frameDelta, settings.speedModifier, settings.rotationModifier);

        if (started) {
            Draw(renderer, player.camera, level, &wallTextureBuffers, numWallTextures, wallTexSize, frameTexture);
        } else {
            SDL_Delay(1);
        }

        started = true;
    }

    Quit(window, renderer);

    return 0;
}