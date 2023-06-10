#define SDL_MAIN_HANDLED

#include <iostream>
#include <vector>

#include <SDL.h>

#include "Level.h"
#include "Player.h"
#include "Renderer.h"
#include "Settings.h"
#include "Utilities.h"
#include "Menu.h"

using namespace MiniFPS;

int main() {
    if (!InitializeSDL()) {
        std::cerr << "SDL could not be initialized:" << SDL_GetError();
    }

    if (!InitializeSDLImage()) {
        std::cerr << "SDL_image could not be initialized" << std::endl;
    }
    
    if (!InitializeSDLTTF()) {
        std::cerr << "SDL_ttf could not be initialized" << std::endl;
    }

    SDL_Window* window = nullptr;
    SDL_Renderer* sdlRenderer = nullptr;

    Settings settings = Settings::LoadSettings(GetSDLAssetsFolderPath(), "settings.json");

    Font fonts[settings.fontPaths.size()];
    for (size_t i = 0; i < settings.fontPaths.size(); i++) {
        fonts[i] = Font(settings.fontPaths[i].first, GetSDLAssetsFolderPath() + settings.fontPaths[i].second, 24);
    }

    std::vector<std::string> spriteFileNames = GetFilesInDirectory(GetSDLAssetsFolderPath() + "sprites/");
    std::map<std::string, Texture> textureNameToTextureMap;

    for (const auto& file : spriteFileNames) {
        std::string name = file;
        std::string pngExtension = ".png";
        size_t pos = name.find(pngExtension);

        // Strip file extension
        if (pos != std::string::npos) {
            name.erase(pos, pngExtension.size());
        }

        Texture newTexture(name, GetSDLAssetsFolderPath() + "sprites/" + file);
        textureNameToTextureMap[name] = newTexture;
    }

    if (!InitializeWindowAndRenderer(&window, &sdlRenderer, settings.screenWidth, settings.screenHeight, settings.vSync)) {
        std::cerr << "Window and/or renderer could not be initialized" << std::endl;
    }

    Renderer renderer(sdlRenderer, settings);

    // Allow movement of cursor in menu
    SDL_SetRelativeMouseMode(SDL_FALSE);
    MainMenu mainMenu(settings);

    while (mainMenu.player.InMainMenu() && !mainMenu.player.GameHasEnded()) {
        renderer.DrawMainMenu(settings, fonts[0], mainMenu.player.camera);
        mainMenu.player.Update(0, 0, 0);
    }

    Level level = Level(GetSDLAssetsFolderPath() + settings.levelPath);

    std::unordered_map<short, Texture> textureMap;
    for (const auto& pair : textureNameToTextureMap) {
        for (const auto& x : level.textureIdMap) {
            if (x.second == pair.first) {
                textureMap[x.first] = pair.second;
            }
        }
    }

    textureMap[-1] = textureNameToTextureMap["fallback"];

    Player gamePlayer(&level, settings);

    renderer.textureMap = textureMap;

    float oldTime, curTime, frameDelta;
    curTime = 0;

    // Disable movement of cursor in game
    SDL_SetRelativeMouseMode(SDL_TRUE);

    while (!gamePlayer.GameHasEnded()) {
        oldTime = curTime;
        curTime = static_cast<float>(SDL_GetTicks64());

        frameDelta = GetFrameTime(oldTime, curTime);

        gamePlayer.Update(frameDelta, settings.speedModifier, settings.rotationModifier);

        renderer.Draw(gamePlayer);
    }

    Quit(window, sdlRenderer);

    return 0;
}