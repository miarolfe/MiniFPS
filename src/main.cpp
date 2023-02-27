#define SDL_MAIN_HANDLED
#include <iostream>
#include <cmath>

#include <SDL.h>
#include <SDL_image.h>

#include "Camera.h"
#include "Color.h"
#include "Level.h"
#include "Renderer.h"

// TODO: Separate rendering from update logic
// TODO: Textured walls

// ~60 fps
//const uint8_t FRAME_INTERVAL = 17;

// ~120 fps
const uint8_t FRAME_INTERVAL = 8;

// TODO: Multiple resolutions
const size_t SCREEN_WIDTH = 1280;
const size_t SCREEN_HEIGHT = 720;

const float RENDER_RAY_INCREMENT = 0.005f;
const size_t RENDER_DISTANCE = 128;

const float ROTATION_MODIFIER = 0.025;
const float DELTA = 0.05;

// TEMP - move this to separate file

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

    const char * platform = SDL_GetPlatform();

    Level level = Level(nullptr);

    if (strcmp(platform, "Windows") == 0) {
        level = Level("assets/levels/testLevel9.png");
    } else if (strcmp(platform, "Mac OS X") == 0) {
        level = Level("../Resources/levels/testLevel9.png");
    } else {
        std::cerr << "Invalid platform: " << platform << std::endl;
        return -1;
    }

    level.print();

    bool gameIsRunning = true;
    int x, y, mouseX, mouseY;
    float deltaM;
    bool moveLeft = false;
    bool moveRight = false;

    Camera playerCamera(2, 2, 1.523, (70.0/360.0) * 2 * M_PI, SCREEN_WIDTH, SCREEN_HEIGHT, RENDER_RAY_INCREMENT, RENDER_DISTANCE, 1);

    SDL_SetRelativeMouseMode(SDL_TRUE);
    
    while (gameIsRunning) {
        // std::cout << "(" << playerCamera.x << ", " << playerCamera.y << ")" << std::endl;

        deltaM = 0;
        mouseX = 0;
        mouseY = 0;
        handle_input(gameIsRunning, x, y, deltaM, moveLeft, moveRight, mouseX, mouseY);
        
        int roundedPlayerCameraX = round(playerCamera.x);
        int roundedPlayerCameraY = round(playerCamera.y);

        float prevPlayerCameraX = playerCamera.x;
        float prevPlayerCameraY = playerCamera.y;

        playerCamera.x += deltaM * cos(playerCamera.angle);
        playerCamera.y += deltaM * sin(playerCamera.angle);
        playerCamera.angle += mouseX * DELTA * ROTATION_MODIFIER;

        if (moveLeft) {
            playerCamera.x += DELTA * cos(playerCamera.angle - M_PI / 2);
            playerCamera.y += DELTA * sin(playerCamera.angle - M_PI / 2);
        }

        if (moveRight) {
            playerCamera.x += DELTA * cos(playerCamera.angle + M_PI / 2);
            playerCamera.y += DELTA * sin(playerCamera.angle + M_PI / 2);
        }

        // Wall collision detection
        for (size_t nearbyX = roundedPlayerCameraX-1; nearbyX <= roundedPlayerCameraX+1; nearbyX++) {
            for (size_t nearbyY = roundedPlayerCameraY-1; nearbyY <= roundedPlayerCameraY+1; nearbyY++) {
                if (level.get(nearbyX, nearbyY) != RGBA_WHITE) {
                    if (playerCamera.x >= nearbyX && playerCamera.x <= nearbyX+1 && playerCamera.y >= nearbyY && playerCamera.y <= nearbyY+1) {
                        playerCamera.x = prevPlayerCameraX;
                        playerCamera.y = prevPlayerCameraY;
                    }
                }
            }
        }

        draw(renderer, playerCamera, level);

        // TODO: Better frame timing solution
        SDL_Delay(FRAME_INTERVAL);
    }

    quit(window);

    return 0;
}