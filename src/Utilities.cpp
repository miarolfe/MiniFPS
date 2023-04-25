#include <iostream>
#include <string>

#include <SDL.h>
#include <SDL_image.h>

#include "Utilities.h"

// Initialize SDL subsystems
bool InitializeSDL() {
    bool successful_initialization = true;

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        successful_initialization = false;
    }

    return successful_initialization;
}

// Initialise window and renderer with parameters
bool InitializeWindowAndRenderer(SDL_Window** window, SDL_Renderer** renderer, size_t screenWidth,
                                 size_t screenHeight, bool vSync) {
    bool successful_initialization = true;

    *window = SDL_CreateWindow("mini-fps",
                               100,
                               100,
                               screenWidth,
                               screenHeight,
                               SDL_WINDOW_SHOWN);

    if (*window == nullptr) {
        successful_initialization = false;
    }

    if (vSync) {
        *renderer = SDL_CreateRenderer(*window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    } else {
        *renderer = SDL_CreateRenderer(*window, -1, SDL_RENDERER_ACCELERATED);
    }

    if (*renderer == nullptr) {
        successful_initialization = false;
    }

    // Capture mouse cursor and enable relative mouse mode
    SDL_SetWindowGrab(*window, SDL_TRUE);
    SDL_SetRelativeMouseMode(SDL_TRUE);

    return successful_initialization;
}

// Initialise SDL_image subsystem
bool InitializeSDLImage() {
    bool successful_initialization = true;

    if (!IMG_Init(IMG_INIT_PNG)) {
        successful_initialization = false;
    }

    return successful_initialization;
}

// Shut down SDL subsystems and free window + renderer
void Quit(SDL_Window* window, SDL_Renderer* renderer) {
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
}

// Get frames per second as string
std::string GetFramesPerSecond(const double oldTime, const double curTime) {
    return std::to_string(static_cast<int>((1.0 / GetFrameTime(oldTime, curTime))));
}

// Get time between frames in seconds
double GetFrameTime(const double oldTime, const double curTime) {
    double frameTime = (curTime - oldTime) / 1000.0;
    return frameTime;
}

// Get platform-appropriate path to assets folder
std::string GetAssetsFolderPath() {
    std::string file_path;
    const char* platform = SDL_GetPlatform();

    // strcmp returns 0 if two strings are identical
    if (strcmp(platform, "Windows") == 0) {
        file_path = "assets/";
    } else if (strcmp(platform, "Mac OS X") == 0) {
        file_path = "../Resources/";
    } else {
        std::cerr << "Invalid platform: " << platform << std::endl;
        file_path = "INVALID PLATFORM";
    }

    return file_path;
}

// Get a texture in Uint3w2 buffer form
bool LoadTextureToBuffer(Uint32*** buffer, size_t& size, std::string assetsFolderPath, std::string textureFilePath) {
    bool success = true;

    std::string fullTexturePath = assetsFolderPath + textureFilePath;
    SDL_Surface* tempTextureSurface = IMG_Load(fullTexturePath.c_str());

    tempTextureSurface = SDL_ConvertSurfaceFormat(tempTextureSurface, SDL_PIXELFORMAT_ARGB8888, 0);

    if (tempTextureSurface == nullptr) {
        std::cerr << "Could not load texture at " << fullTexturePath << ": " << IMG_GetError();
        success = false;
    }

    if (success && tempTextureSurface->w != tempTextureSurface->h) {
        std::cerr << "Could not use texture at " << fullTexturePath << ", texture must be square" << std::endl;
        success = false;
    }

    if (success) {
        // We only use square textures so h doesn't matter
        size = tempTextureSurface->w;
        *buffer = new Uint32*[size];
        for (int row = 0; row < size; row++) {
            (*buffer)[row] = new Uint32[size];
        }

        Uint32* pixels = (Uint32*) tempTextureSurface->pixels;

        for (int p = 0; p < size; p++) {
            for (int q = 0; q < size; q++) {
                (*buffer)[p][q] = pixels[p * size + q];
            }
        }

        SDL_FreeSurface(tempTextureSurface);

        std::cout << "Loaded texture to buffer: " + textureFilePath << std::endl;
    }

    return success;
}

// Get a clamped version of a value
int Clamp(const int value, const int min, const int max) {
    int clampedValue;

    if (value < min) {
        clampedValue = min;
    } else if (value > max) {
        clampedValue = max;
    } else {
        clampedValue = value;
    }

    return clampedValue;
}
