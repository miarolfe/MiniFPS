#include <iostream>
#include <SDL.h>

// ~60 fps
const uint8_t FRAME_INTERVAL = 17;

// TODO: Multiple resolutions
const size_t SCREEN_WIDTH = 1024;
const size_t SCREEN_HEIGHT = 512;

void initialize_window_and_renderer(SDL_Window** window, SDL_Renderer** renderer) {
    if (SDL_Init(SDL_INIT_VIDEO < 0)) {
        std::cout << "SDL video system could not be initialized: " << SDL_GetError();
    } else {
        std::cout << "SDL video system initialized" << std::endl;
    }

    *window  = SDL_CreateWindow("mini-fps",
            0,
            0,
            SCREEN_WIDTH,
            SCREEN_HEIGHT,
            SDL_WINDOW_SHOWN);

    *renderer = SDL_CreateRenderer(*window, -1, SDL_RENDERER_ACCELERATED);
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
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;

    initialize_window_and_renderer(&window, &renderer);

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