#define SDL_MAIN_HANDLED

#include <iostream>
#include <vector>

#include <SDL.h>

#include "Camera.h"
#include "Color.h"
#include "Level.h"
#include "Player.h"
#include "Renderer.h"
#include "Settings.h"
#include "Utilities.h"

void WriteLineToFile(std::string fileName, std::string line) {
    std::ofstream outFile(fileName, std::ios::app); // open file in append mode
    outFile << line << std::endl; // write line and append newline character
    outFile.close(); // close file
}

int main() {
    if (!InitializeSDL()) {
        std::cerr << "SDL could not be initialized:" << SDL_GetError();
        WriteLineToFile("log.txt", "SDL could not be initialized");
    } else {
        std::cout << "SDL initialized" << std::endl;
        WriteLineToFile("log.txt", "SDL initialized");
    }

    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    const std::string assetsFolderPath = GetAssetsFolderPath();
    WriteLineToFile("log.txt", assetsFolderPath);

    std::vector<std::string> texturePaths = {"sprites/white-bricks.png",
                                             "sprites/red-bricks.png",
                                             "sprites/yellow-bricks.png",
                                             "sprites/green-bricks.png",
                                             "sprites/cyan-bricks.png",
                                             "sprites/blue-bricks.png",
                                             "sprites/indigo-bricks.png",
                                             "sprites/grey-bricks.png"};

    //Settings settings = LoadSettings(assetsFolderPath, "settings.json");
    Settings settings = Settings(400, 400, 0.005, 128, true, 90, 5, 0.5, 1.5, 2, 1.523, 0.5, "levels/test.lvl", texturePaths);

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

    std::string levelFilePath = assetsFolderPath + settings.levelPath;

    size_t wallTexSize;
    // TODO: Allow variable size textures
    // Right now all wall textures must be the same size
    size_t numWallTextures = settings.texturePaths.size();
    Uint32*** wallTextureBuffers = new Uint32** [numWallTextures];
    for (size_t buffer = 0; buffer < numWallTextures; buffer++) {
        LoadTextureToBuffer(&wallTextureBuffers[buffer], wallTexSize, assetsFolderPath,
                            settings.texturePaths[buffer]);
    }

    Level level = Level(levelFilePath);
    level.Print();

    bool started = false;
    bool gameIsRunning = true;
    int mouseX, mouseY;
    bool moveLeft, moveRight, moveForward, moveBack;

    Camera playerCamera(settings.playerStartX, settings.playerStartY, settings.playerStartAngle,
                        settings.fieldOfView * PI_180, settings.screenWidth,
                        settings.screenHeight, settings.renderRayIncrement,
                        settings.renderDistance, settings.playerDistanceToProjectionPlane);


    double oldTime, curTime, frameDelta;
    curTime = 0;

    SDL_Texture* frameTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING,
                                                  playerCamera.viewportWidth, playerCamera.viewportHeight);

    while (gameIsRunning) {
        oldTime = curTime;
        curTime = static_cast<double>(SDL_GetTicks64());

        frameDelta = GetFrameTime(oldTime, curTime);

        mouseX = 0;
        mouseY = 0;
        GetInputState(gameIsRunning, moveLeft, moveRight, moveForward, moveBack, mouseX, mouseY);

        float prevPlayerCameraX = playerCamera.x;
        float prevPlayerCameraY = playerCamera.y;
        float prevPlayerCameraAngle = playerCamera.angle;

        playerCamera.angle += mouseX * frameDelta * settings.rotationModifier;
        Move(playerCamera, frameDelta, settings.speedModifier, moveLeft, moveRight, moveForward, moveBack);

        if (level.HasCollided(playerCamera.x, playerCamera.y)) {
            playerCamera.x = prevPlayerCameraX;
            playerCamera.y = prevPlayerCameraY;
        }

        // Only rerender the screen if something's changed
        // TODO: Update this when animated sprites/enemies in game
        if (playerCamera.angle != prevPlayerCameraAngle || playerCamera.x != prevPlayerCameraX ||
            playerCamera.y != prevPlayerCameraY || !started) {
            Draw(renderer, playerCamera, level, &wallTextureBuffers, numWallTextures, wallTexSize, frameTexture);
        } else {
            SDL_Delay(1);
        }

        started = true;
    }

    Quit(window, renderer);

    return 0;
}