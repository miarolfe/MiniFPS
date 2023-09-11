#include <iostream>

#include "Game.h"
#include "Utilities.h"

MiniFPS::Game::Game() {
    InitializeSDLSubsystems();

    settings = Settings::LoadSettings(GetSDLAssetsFolderPath(), "settings.json");
    audio = Audio(GetSDLAssetsFolderPath() + "audio/", settings);
    fontManager = FontManager(settings);

    LoadTextures();

    if (!InitializeWindowAndRenderer(&window, &sdlRenderer, settings.screenWidth, settings.screenHeight, settings.vSync)) {
        std::cerr << "Window and/or renderer could not be initialized" << std::endl;
    }

    renderer = Renderer(sdlRenderer, settings);
    SDL_SetRelativeMouseMode(SDL_FALSE);
    mainMenu = MainMenu(settings, fontManager.fonts[0]);
}

void MiniFPS::Game::Update() {

}

bool MiniFPS::Game::IsRunning() {
    return false;
}

// TODO
MiniFPS::Game::~Game() {

}

void MiniFPS::Game::LoadTextures() {
    const std::vector<std::string> spriteFileNames = GetFilesInDirectory(GetSDLAssetsFolderPath() + "sprites/");

    for (const auto& file : spriteFileNames) {
        const std::string name = file.substr(0, file.size()-4);

        Texture newTexture(name, GetSDLAssetsFolderPath() + "sprites/" + file);
        textureNameToTextureMap[name] = newTexture;
    }
}
