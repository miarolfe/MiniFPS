#define SDL_MAIN_HANDLED

#include <iostream>

#include "Game.h"

using namespace MiniFPS;

int main() {
    Game game;
    while (game.IsRunning()) {
        game.Update();
    }
    /*
    InitializeSDLSubsystems();

    SDL_Window* window = nullptr;
    SDL_Renderer* sdlRenderer = nullptr;

    Settings settings = Settings::LoadSettings(GetSDLAssetsFolderPath(), "settings.json");

    Audio audio(GetSDLAssetsFolderPath() + "audio/", settings);

    FontManager fontManager(settings);

    std::map<std::string, Texture> textureNameToTextureMap;
    const std::vector<std::string> spriteFileNames = GetFilesInDirectory(GetSDLAssetsFolderPath() + "sprites/");

    for (const auto& file : spriteFileNames) {
        const std::string name = file.substr(0, file.size()-4);

        Texture newTexture(name, GetSDLAssetsFolderPath() + "sprites/" + file);
        textureNameToTextureMap[name] = newTexture;
    }

    if (!InitializeWindowAndRenderer(&window, &sdlRenderer, settings.screenWidth, settings.screenHeight, settings.vSync)) {
        std::cerr << "Window and/or renderer could not be initialized" << std::endl;
    }

    Renderer renderer(sdlRenderer, settings);

    // Allow movement of cursor in menu
    SDL_SetRelativeMouseMode(SDL_FALSE);
    MainMenu mainMenu(settings, fontManager.fonts[0]);

    while (mainMenu.player.InMainMenu() && !mainMenu.player.GameHasEnded()) {
        renderer.DrawMainMenu(mainMenu);
        mainMenu.Update();
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
    gamePlayer.weaponTexture = textureNameToTextureMap["Player_Weapon"];

    renderer.SetTextureMap(textureMap);

    float oldTime = 0.0;
    float curTime = 0.0;
    float frameDelta = 0.0;

    // Disable movement of cursor in game
    SDL_SetRelativeMouseMode(SDL_TRUE);

    audio.PlayTrack("DrumLoop1", -1);

    while (!gamePlayer.GameHasEnded()) {
        oldTime = curTime;
        curTime = static_cast<float>(SDL_GetTicks64());

        frameDelta = GetFrameTime(oldTime, curTime);

        gamePlayer.Update(frameDelta, settings.speedModifier, settings.rotationModifier);

        if (gamePlayer.inputState.leftMouseButtonPressed) {
            audio.PlayEffect("GunShoot1");
        }

        renderer.Draw(gamePlayer, fontManager.fonts[0], frameDelta);
    }

    FreeResources(renderer, audio, fontManager);
    DeactivateSDLSubsystems();
    Quit(window, sdlRenderer);
    */

    return 0;
}