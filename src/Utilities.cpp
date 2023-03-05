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

bool initialize_window_and_renderer(SDL_Window **window, SDL_Renderer **renderer, const size_t SCREEN_WIDTH, const size_t SCREEN_HEIGHT) {
    bool successful_initialization = true;

    *window = SDL_CreateWindow("mini-fps",
                               100,
                               100,
                               SCREEN_WIDTH,
                               SCREEN_HEIGHT,
                               SDL_WINDOW_SHOWN);

    if (*window == nullptr) {
        successful_initialization = false;
    }

    *renderer = SDL_CreateRenderer(*window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

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
    double frameTime = (curTime - oldTime) / 1000.0;
    return "FPS: " + std::to_string(static_cast<int>((1.0 / frameTime))) + "\n";
}
