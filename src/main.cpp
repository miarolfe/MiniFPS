#include <iostream>
#include <SDL.h>

const uint8_t FRAME_INTERVAL = 17;

// TODO: Multiple resolutions
const size_t SCREEN_WIDTH = 1024;
const size_t SCREEN_HEIGHT = 512;

// Global variables
SDL_Window* gWindow;
SDL_Renderer* gRenderer;

void initialization() {
    if (SDL_Init(SDL_INIT_VIDEO < 0)) {
        std::cout << "SDL video system could not be initialized: " << SDL_GetError();
    } else {
        std::cout << "SDL video system initialized" << std::endl;
    }

    gWindow = SDL_CreateWindow("C++ SDL2 Window",
            0,
            0,
            SCREEN_WIDTH,
            SCREEN_HEIGHT,
            SDL_WINDOW_SHOWN);

    gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
}

int main() {
    gWindow = nullptr;
    gRenderer = nullptr;

    initialization();

    bool drawSquare = false;
    bool gameIsRunning = true;
    
    while (gameIsRunning) {
        SDL_SetRenderDrawColor(gRenderer, 255, 0, 0, SDL_ALPHA_OPAQUE);
        SDL_RenderClear(gRenderer);

        SDL_Event event;

        int x, y;
        SDL_GetMouseState(&x,&y);

        while (SDL_PollEvent(&event)){ 
            // Should this be a switch statement?
            if (event.type == SDL_QUIT) {
                gameIsRunning= false;
            }

            if (event.type == SDL_MOUSEBUTTONDOWN) {
                drawSquare = !drawSquare;
            }
        }

        // Rendering
        if (drawSquare) {
            SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);

            // TODO: Replace this with actual rectangle
            // Listen I'm still learning

            SDL_Rect rect;

            rect.x = x-12;
            rect.y = y-12;
            rect.w = 24;
            rect.h = 24;
            for (int i = -25; i < 25; i++) {
                SDL_RenderDrawRect(gRenderer, &rect);
            }
        }

        // TODO: Better frame timing solution
        SDL_Delay(FRAME_INTERVAL);

        SDL_RenderPresent(gRenderer);        
    }

    SDL_DestroyWindow(gWindow);
    SDL_Quit();

    return 0;
}