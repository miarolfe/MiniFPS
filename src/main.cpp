#include <iostream>

#include <SDL.h>
#include <SDL_image.h>

#include "Camera.h"
#include "Color.h"
#include "Level.h"

// ~60 fps
//const uint8_t FRAME_INTERVAL = 17;

// ~120 fps
const uint8_t FRAME_INTERVAL = 8;

// TODO: Multiple resolutions
const size_t SCREEN_WIDTH = 800;
const size_t SCREEN_HEIGHT = 600;

const float RENDER_RAY_INCREMENT = 0.005f;
const size_t RENDER_DISTANCE = 128;

const float ROTATION_MODIFIER = 0.025;
const float DELTA = 0.05;

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

void handle_input(bool& gameIsRunning, int& x, int& y, float& deltaM, bool& moveLeft, bool& moveRight, int& mouseX, int &mouseY) {
    SDL_Event event;
    SDL_GetMouseState(&x, &y);

    while (SDL_PollEvent(&event)) {
        // Should this be a switch statement?
        if (event.type == SDL_QUIT) {
            gameIsRunning = false;
        }

        if (event.type == SDL_MOUSEMOTION)
        {
            mouseX = event.motion.xrel;
            mouseY = event.motion.yrel;
        }
    }

    const Uint8 *currentKeyStates = SDL_GetKeyboardState(nullptr);

    if (currentKeyStates[SDL_SCANCODE_ESCAPE]) {
        gameIsRunning = false;
    }

    if (currentKeyStates[SDL_SCANCODE_W]) {
        deltaM = DELTA;
    }

    if (currentKeyStates[SDL_SCANCODE_S]) {
        deltaM = -DELTA;
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
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(renderer);

    for (size_t ray = 0; ray < SCREEN_WIDTH; ray++) {
        float rayAngle = (camera.angle - camera.fieldOfView / 2) + (camera.fieldOfView * ray / float(SCREEN_WIDTH));

        float t;
        for (t = 0; t < RENDER_DISTANCE; t += RENDER_RAY_INCREMENT) {
            float cx = camera.x + t * cos(rayAngle);
            float cy = camera.y + t * sin(rayAngle);

            if (level.get(int(cx), int(cy)) != RGBA_WHITE) {
                Uint8 r, g, b, a;
                SDL_GetRGBA(level.get(int(cx), int(cy)), &level.pixelFormat, &r, &g, &b, &a);
                SDL_SetRenderDrawColor(renderer, r, g, b, a);

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

void quit(SDL_Window* window) {
    SDL_DestroyWindow(window);
    SDL_Quit();
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

    Level level("../Resources/levels/testLevel7.png");

    level.print();

    bool gameIsRunning = true;
    int x, y, mouseX, mouseY;
    float deltaM;
    bool moveLeft = false;
    bool moveRight = false;

    Camera camera(2, 2, 1.523, 3*M_PI/10.0);

    SDL_SetRelativeMouseMode(SDL_TRUE);
    
    while (gameIsRunning) {
        std::cout << "(" << camera.x << ", " << camera.y << ")" << std::endl;

        deltaM = 0;
        mouseX = 0;
        mouseY = 0;
        handle_input(gameIsRunning, x, y, deltaM, moveLeft, moveRight, mouseX, mouseY);

        // Collision detection idea
        // if abs(camera.x - round(camera.x)) < THRESHOLD
        //      ...

        camera.x += deltaM * cos(camera.angle);
        camera.y += deltaM * sin(camera.angle);
        camera.angle += mouseX * DELTA * ROTATION_MODIFIER;

        if (moveLeft) {
            camera.x += DELTA * cos(camera.angle - M_PI / 2);
            camera.y += DELTA * sin(camera.angle - M_PI / 2);
        }

        if (moveRight) {
            camera.x += DELTA * cos(camera.angle + M_PI / 2);
            camera.y += DELTA * sin(camera.angle + M_PI / 2);
        }

        draw(renderer, camera, level);

        // TODO: Better frame timing solution
        SDL_Delay(FRAME_INTERVAL);
    }

    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}