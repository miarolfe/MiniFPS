#include "Game.h"

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
    if (mainMenu.player.InMainMenu() && !mainMenu.player.GameHasEnded()) {
        renderer.DrawMainMenu(mainMenu);
        mainMenu.Update();
    } else if (!gamePlayer.GameHasEnded()) {
        if (!gameSetup) {
            SetupGame();
            gameSetup = true;
        }

        oldTime = curTime;
        curTime = static_cast<float>(SDL_GetTicks64());

        frameDelta = GetFrameTime(oldTime, curTime);

        gamePlayer.Update(frameDelta, settings.speedModifier, settings.rotationModifier);

        if (gamePlayer.inputState.leftMouseButtonPressed) {
            audio.PlayEffect("GunShoot1");
        }

        renderer.Draw(gamePlayer, fontManager.fonts[0], frameDelta);
    }
}

bool MiniFPS::Game::IsRunning() {
    return !gamePlayer.GameHasEnded();
}

MiniFPS::Game::~Game() {
    FreeResources(renderer, audio, fontManager);
    DeactivateSDLSubsystems();
    Quit(window, sdlRenderer);
}

void MiniFPS::Game::LoadTextures() {
    const std::vector<std::string> spriteFileNames = GetFilesInDirectory(GetSDLAssetsFolderPath() + "sprites/");

    for (const auto& file : spriteFileNames) {
        const std::string name = file.substr(0, file.size()-4);

        const Texture newTexture(name, GetSDLAssetsFolderPath() + "sprites/" + file);
        textureNameToTextureMap[name] = newTexture;
    }
}

void MiniFPS::Game::SetupGame() {
    level = Level(GetSDLAssetsFolderPath() + settings.levelPath);

    for (const auto& pair : textureNameToTextureMap) {
        for (const auto& x : level.textureIdMap) {
            if (x.second == pair.first) {
                textureMap[x.first] = pair.second;
            }
        }
    }

    textureMap[-1] = textureNameToTextureMap["fallback"];

    gamePlayer = Player(&level, settings);
    gamePlayer.weaponTexture = textureNameToTextureMap["Player_Weapon"];

    renderer.SetTextureMap(textureMap);

    // Disable movement of cursor in game
    SDL_SetRelativeMouseMode(SDL_TRUE);

    audio.PlayTrack("DrumLoop1", -1);
}
