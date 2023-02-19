#include <iostream>
#include <SDL.h>
#include <SDL_image.h>

#include "Level.h"

// ~60 fps
const uint8_t FRAME_INTERVAL = 17;

// TODO: Multiple resolutions
const size_t SCREEN_WIDTH = 1024;
const size_t SCREEN_HEIGHT = 512;

// Color constants
const Uint32 RGBA_BLACK = 0xFF000000;
const Uint32 RGBA_WHITE = 0xFFFFFFFF;

bool initialize_sdl() {
    bool successful_initialization = true;

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        successful_initialization = false;
    }

    return successful_initialization;
}

bool initialize_window_and_renderer(SDL_Window** window, SDL_Renderer** renderer) {
    bool successful_initialization = true;

    *window = SDL_CreateWindow("mini-fps",
            0,
            0,
            SCREEN_WIDTH,
            SCREEN_HEIGHT,
            SDL_WINDOW_SHOWN);

    if (*window == nullptr) {
        successful_initialization = false;
    }

    *renderer = SDL_CreateRenderer(*window, -1, SDL_RENDERER_ACCELERATED);

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

void handle_input(bool& drawSquare, bool& gameIsRunning, int& x, int& y) {
    SDL_Event event;
    SDL_GetMouseState(&x, &y);

    while (SDL_PollEvent(&event)){ 
        // Should this be a switch statement?
        if (event.type == SDL_QUIT) {
            gameIsRunning= false;
        }

        if (event.type == SDL_MOUSEBUTTONDOWN) {
            drawSquare = !drawSquare;
        }
    }
}

void draw(SDL_Renderer* renderer, const bool drawSquare, const int x, const int y) {
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(renderer);
        
    if (drawSquare) {
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

        SDL_Rect rect;
        rect.x = x-12;
        rect.y = y-12;
        rect.w = 24;
        rect.h = 24;
        SDL_RenderFillRect(renderer, &rect);
    }

    SDL_RenderPresent(renderer);     
}

int main() {
    if (!initialize_sdl()) {
        std::cerr << "SDL could not be initialized:" << SDL_GetError();
    } else {
        std::cout << "SDL initialized" << std::endl;
    }

    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;

    if (!initialize_window_and_renderer(&window, &renderer)) {
        std::cerr << "Window and/or renderer could not be initialized" << std::endl;
    } else {
        std::cout << "Window and renderer initialized" << std::endl;
    }

    if (!initialize_sdl_image()) {
        std::cerr << "SDL_image could not be initialized" << std::endl;
    } else {
        std::cout << "SDL_image initialized" << std::endl;
    }

    Level level("../assets/testMap2.png");
    level.print();

    bool drawSquare = false;
    bool gameIsRunning = true;
    int x, y;
    
    while (gameIsRunning) {
        handle_input(drawSquare, gameIsRunning, x, y);
        draw(renderer, drawSquare, x, y);

        // TODO: Better frame timing solution
        SDL_Delay(FRAME_INTERVAL);   
    }

    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}