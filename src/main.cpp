#include <iostream>
#include <SDL.h>

const uint8_t FRAME_INTERVAL = 17;

// TODO: Multiple resolutions
const size_t SCREEN_WIDTH = 1024;
const size_t SCREEN_HEIGHT = 512;

int main() {
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;

    if (SDL_Init(SDL_INIT_VIDEO < 0)) {
        std::cout << "SDL video system could not be initialized: " << SDL_GetError();
        return -1;
    } else {
        std::cout << "SDL video system initialized" << std::endl;
    }

    window = SDL_CreateWindow("C++ SDL2 Window",
            0,
            0,
            SCREEN_WIDTH,
            SCREEN_HEIGHT,
            SDL_WINDOW_SHOWN);

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    bool drawSquare = false;
    bool gameIsRunning = true;
    
    while(gameIsRunning) {
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, SDL_ALPHA_OPAQUE);
        SDL_RenderClear(renderer);

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
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

            // TODO: Replace this with actual rectangle
            // Listen I'm still learning
            for (int i = -25; i < 25; i++) {
                SDL_RenderDrawLine(renderer, x-25, y+i, x+25, y+i);
            }
        }

        // TODO: Better frame timing solution
        SDL_Delay(FRAME_INTERVAL);

        SDL_RenderPresent(renderer);        
    }

    SDL_DestroyWindow(window);

    SDL_Quit();
    return 0;
}