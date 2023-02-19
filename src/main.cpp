#include <iostream>
#include <SDL.h>
#include <SDL_image.h>

#include "Camera.h"
#include "Level.h"

// ~60 fps
const uint8_t FRAME_INTERVAL = 17;

// TODO: Multiple resolutions
const size_t SCREEN_WIDTH = 1920;
const size_t SCREEN_HEIGHT = 1080;

// Color constants
const Uint32 RGBA_BLACK = 0xFF000000;
const Uint32 RGBA_WHITE = 0xFFFFFFFF;

const float delta = 0.05;

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

void handle_input(bool& drawSquare, bool& gameIsRunning, int& x, int& y, float& deltaX, float& deltaY) {
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

//        if (event.type == SDL_KEYDOWN) {
//            if (event.key.keysym.sym == SDLK_w) {
//                deltaY = delta;
//            } else if (event.key.keysym.sym == SDLK_s) {
//                deltaY = -delta;
//            } else if (event.key.keysym.sym == SDLK_a) {
//                deltaX = -delta;
//            } else if (event.key.keysym.sym == SDLK_d) {
//                deltaX = delta;
//            }
//        }
    }

    const Uint8* currentKeyStates = SDL_GetKeyboardState( NULL );

    if (currentKeyStates[SDL_SCANCODE_W]) {
        deltaY = delta;
    }

    if (currentKeyStates[SDL_SCANCODE_S]) {
        deltaY = -delta;
    }

    if (currentKeyStates[SDL_SCANCODE_A]) {
        deltaX = -delta;
    }

    if (currentKeyStates[SDL_SCANCODE_D]) {
        deltaX = delta;
    }
}

void draw(SDL_Renderer* renderer, Camera camera, Level& level) {
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
    const size_t rect_w = SCREEN_WIDTH / level.w;
    const size_t rect_h = SCREEN_HEIGHT / level.h;

    for (size_t ray = 0; ray < SCREEN_WIDTH; ray++) {
        float rayAngle = (camera.angle - camera.fieldOfView / 2) + (camera.fieldOfView * ray / float(SCREEN_WIDTH));
        // std::cout << "Ray angle: " << rayAngle << std::endl;
        // std::cout << "Camera angle: " << camera.angle << std::endl;

        float t;
        for (t = 0; t < 20; t += 0.01) {
            float cx = camera.x + t * cos(rayAngle);
            float cy = camera.y + t * sin(rayAngle);

            if (level.get(int(cx), int(cy)) == RGBA_BLACK) {
                // std::cout << "RGBA Black found" << std::endl;
                // std::cout << cos(rayAngle - camera.angle) << std::endl;
                // std::cout << t << std::endl;
d
                // std::cout << (t * cos(rayAngle - camera.angle)) << std::endl;
                size_t column_height = float(SCREEN_HEIGHT)/t * cos(rayAngle-camera.angle);
                SDL_Rect column;
                column.x = ray;
                column.y = SCREEN_HEIGHT/2 - column_height/2;
                column.w = 1;
                column.h = column_height;

                // std::cout << column.x << " " << column.y << " " << column.w << " " << column.h << std::endl;
                SDL_RenderFillRect(renderer, &column);
                break;
            }
        }

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

    Level level("../assets/testMap1.png");
    level.print();

    bool drawSquare = false;
    bool gameIsRunning = true;
    int x, y;
    float deltaX, deltaY;
    Camera camera(3.456, 2.345, 1.523, M_PI/2);
    // Camera camera(32, 32, 1.523, M_PI/3.);
    
    while (gameIsRunning) {
        deltaX = 0;
        deltaY = 0;
        handle_input(drawSquare, gameIsRunning, x, y, deltaX, deltaY);
        camera.x += deltaX;
        camera.y += deltaY;
        draw(renderer, camera, level);

        // TODO: Better frame timing solution
        SDL_Delay(FRAME_INTERVAL);
    }

    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}