#include <iostream>
#include <SDL.h>
#include <SDL_image.h>

#include "Camera.h"
#include "Color.h"
#include "Level.h"

// ~60 fps
const uint8_t FRAME_INTERVAL = 17;

// ~120 fps
//const uint8_t FRAME_INTERVAL = 8;

// TODO: Multiple resolutions
const size_t SCREEN_WIDTH = 800;
const size_t SCREEN_HEIGHT = 600;

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

void handle_input(bool& gameIsRunning, int& x, int& y, float& deltaM, float& deltaA, bool& moveLeft, bool& moveRight) {
    SDL_Event event;
    SDL_GetMouseState(&x, &y);

    while (SDL_PollEvent(&event)) {
        // Should this be a switch statement?
        if (event.type == SDL_QUIT) {
            gameIsRunning = false;
        }
    }

    const Uint8 *currentKeyStates = SDL_GetKeyboardState(nullptr);

    if (currentKeyStates[SDL_SCANCODE_W]) {
        deltaM = delta;
    }

    if (currentKeyStates[SDL_SCANCODE_S]) {
        deltaM = -delta;
    }

    if (currentKeyStates[SDL_SCANCODE_Q]) {
        deltaA = -delta;
    }

    if (currentKeyStates[SDL_SCANCODE_E]) {
        deltaA = delta;
    }

    if (currentKeyStates[SDL_SCANCODE_A]) {
        moveLeft = true;
    } else {
        moveLeft = false;
    }

    if (currentKeyStates[SDL_SCANCODE_D]) {
        moveRight = true;
    } else {
        moveRight = false;
    }
}

void draw(SDL_Renderer* renderer, Camera camera, Level& level) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);

    for (size_t ray = 0; ray < SCREEN_WIDTH; ray++) {
        float rayAngle = (camera.angle - camera.fieldOfView / 2) + (camera.fieldOfView * ray / float(SCREEN_WIDTH));

        float t;
        for (t = 0; t < 100; t += 0.005) {
            float cx = camera.x + t * cos(rayAngle);
            float cy = camera.y + t * sin(rayAngle);

            if (level.get(int(cx), int(cy)) == RGBA_BLACK) {
                size_t columnHeight = float(SCREEN_HEIGHT)/t * cos(rayAngle-camera.angle);
                SDL_Rect column;
                column.x = ray;
                column.y = SCREEN_HEIGHT/2 - columnHeight/2;
                column.w = 1;
                column.h = columnHeight;

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

    Level level("../assets/levels/testLevel4.png");
    level.print();

    bool gameIsRunning = true;
    int x, y;
    float deltaM, deltaA;
    bool moveLeft = false;
    bool moveRight = false;

    Camera camera(2, 2, 1.523, 3*M_PI/10.0);
    
    while (gameIsRunning) {
        deltaM = 0;
        deltaA = 0;
        handle_input(gameIsRunning, x, y, deltaM, deltaA, moveLeft, moveRight);
        camera.x += deltaM * cos(camera.angle);
        camera.y += deltaM * sin(camera.angle);

        if (moveLeft) {
            camera.x += delta * cos(camera.angle - M_PI/2);
            camera.y += delta * sin(camera.angle - M_PI/2);
        }

        if (moveRight) {
            camera.x += delta * cos(camera.angle + M_PI/2);
            camera.y += delta * sin(camera.angle + M_PI/2);
        }

        camera.angle += deltaA;
        draw(renderer, camera, level);

        // TODO: Better frame timing solution
        SDL_Delay(FRAME_INTERVAL);
    }

    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}