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
#include "Menu.h"

int main() {
    ClearFile("log.txt");

    if (!InitializeSDL()) {
        WriteLineToFile("log.txt", "SDL could not be initialized");
        std::cerr << "SDL could not be initialized:" << SDL_GetError();
    } else {
        WriteLineToFile("log.txt", "SDL initialized");
        std::cout << "SDL initialized" << std::endl;
    }

    if (!InitializeSDLImage()) {
        WriteLineToFile("log.txt", "SDL_image could not be initialized");
        std::cerr << "SDL_image could not be initialized" << std::endl;
    } else {
        WriteLineToFile("log.txt", "SDL_image initialized");
        std::cout << "SDL_image initialized" << std::endl;
    }

    if (!InitializeSDLTTF()) {
        WriteLineToFile("log.txt", "SDL_ttf could not be initialized");
        std::cerr << "SDL_ttf could not be initialized" << std::endl;
    } else {
        WriteLineToFile("log.txt", "SDL_ttf initialized");
        std::cout << "SDL_ttf initialized" << std::endl;
    }

    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;

    Settings settings = Settings::LoadSettings(GetSDLAssetsFolderPath(), "settings.json");

    WriteLineToFile("log.txt", "Settings loaded");

    Font fonts[settings.fontPaths.size()];
    for (int i = 0; i < settings.fontPaths.size(); i++) {
        fonts[i] = Font(settings.fontPaths[i].first, settings.fontPaths[i].second, 24);
    }

    WriteLineToFile("log.txt", "Fonts loaded");

    Texture textures[settings.texturePaths.size()];
    for (int i = 0; i < settings.texturePaths.size(); i++) {
        textures[i] = Texture(settings.texturePaths[i].first, settings.texturePaths[i].second);
        std::cout << textures[i].name << std::endl;
    }

    WriteLineToFile("log.txt", "Textures loaded");

    if (!InitializeWindowAndRenderer(&window, &renderer, settings.screenWidth, settings.screenHeight, settings.vSync)) {
        WriteLineToFile("log.txt", "Window and/or renderer could not be initialized");
        std::cerr << "Window and/or renderer could not be initialized" << std::endl;
    } else {
        WriteLineToFile("log.txt", "Window and renderer initialized");
        std::cout << "Window and renderer initialized" << std::endl;
    }

    SDL_Texture* streamingFrameTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING,
                                                           static_cast<int>(settings.screenWidth), static_cast<int>(settings.screenHeight));

    SDL_Texture* renderFrameTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_TARGET,
                                                        static_cast<int>(settings.screenWidth), static_cast<int>(settings.screenHeight));

    WriteLineToFile("log.txt", "Textures created");

    // Allow movement of cursor in menu
    SDL_SetRelativeMouseMode(SDL_FALSE);
    MainMenu mainMenu(settings);

    WriteLineToFile("log.txt", "Main menu object created");

    while (mainMenu.player.InMainMenu() && !mainMenu.player.GameHasEnded()) {
        DrawMainMenu(settings, renderer, fonts[0], mainMenu.player.camera, streamingFrameTexture, renderFrameTexture);
        mainMenu.player.Update(0, 0, 0);
    }

    Level level = Level(GetSDLAssetsFolderPath() + settings.levelPath);
    level.Print();

    Player gamePlayer(&level, settings);

    float oldTime, curTime, frameDelta;
    curTime = 0;

    // Disable movement of cursor in game
    SDL_SetRelativeMouseMode(SDL_TRUE);

    while (!gamePlayer.GameHasEnded()) {
        oldTime = curTime;
        curTime = static_cast<float>(SDL_GetTicks64());

        frameDelta = GetFrameTime(oldTime, curTime);

        gamePlayer.Update(frameDelta, settings.speedModifier, settings.rotationModifier);

        Draw(renderer, gamePlayer, textures, settings.texturePaths.size(), streamingFrameTexture, renderFrameTexture);
    }

    Quit(window, renderer);

    return 0;
}