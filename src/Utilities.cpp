#include <iostream>
#include <string>

#include <SDL.h>
#include <SDL_image.h>

#include "Utilities.h"

bool initialize_sdl() {
    bool successful_initialization = true;

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        successful_initialization = false;
    }

    return successful_initialization;
}

bool initialize_window_and_renderer(SDL_Window **window, SDL_Renderer **renderer, const size_t screenWidth, const size_t screenHeight) {
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

    *renderer = SDL_CreateRenderer(*window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    // *renderer = SDL_CreateRenderer(*window, -1, SDL_RENDERER_ACCELERATED);

    if (*renderer == nullptr) {
        successful_initialization = false;
    }

    return successful_initialization;
}

bool initialize_sdl_image() {
    bool successful_initialization = true;

    if (!IMG_Init(IMG_INIT_PNG)) {
        successful_initialization = false;
    }

    return successful_initialization;
}

void quit(SDL_Window *window) {
    SDL_DestroyWindow(window);
    SDL_Quit();
}

std::string frames_per_second(const double oldTime, const double curTime) {
    return "FPS: " + std::to_string(static_cast<int>((1.0 / frame_time(oldTime, curTime)))) + "\n";
}

double frame_time(const double oldTime, const double curTime) {
    double frameTime = (curTime - oldTime) / 1000.0;
    return frameTime;
}

std::string assets_file_path() {
    std::string file_path;
    const char *platform = SDL_GetPlatform();
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
