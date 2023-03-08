// <iostream> needs to be here, or it doesn't compile, complaining about memset_pattern4
#include <iostream>
#include <cmath>

#include <SDL.h>

#include "Camera.h"
#include "Player.h"

void get_input_state(bool &gameIsRunning, bool &moveLeft, bool &moveRight, bool &moveForward,
                     bool &moveBack, int &mouseX, int &mouseY) {
    SDL_Event event;

    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            gameIsRunning = false;
        }

        if (event.type == SDL_MOUSEMOTION) {
            mouseX = event.motion.xrel;
            mouseY = event.motion.yrel;
        }
    }

    const Uint8* currentKeyStates = SDL_GetKeyboardState(nullptr);

    if (currentKeyStates[SDL_SCANCODE_ESCAPE]) {
        gameIsRunning = false;
    }

    if (currentKeyStates[SDL_SCANCODE_W]) {
        moveForward = true;
    } else {
        moveForward = false;
    }

    if (currentKeyStates[SDL_SCANCODE_S]) {
        moveBack = true;
    } else {
        moveBack = false;
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

void move(Camera &camera, const float frameDelta, const float speedModifier, const bool moveLeft, const bool moveRight,
          const bool moveForward, const bool moveBack) {

    // Avoid doing and undoing operations
    if (moveForward != moveBack) {
        if (moveForward) {
            camera.x += frameDelta * speedModifier * cos(camera.angle);
            camera.y += frameDelta * speedModifier * sin(camera.angle);
        }

        if (moveBack) {
            camera.x -= frameDelta * speedModifier * cos(camera.angle);
            camera.y -= frameDelta * speedModifier * sin(camera.angle);
        }
    }

    // Avoid doing and undoing operations
    if (moveLeft != moveRight) {
        if (moveLeft) {
            camera.x += frameDelta * speedModifier * cos(camera.angle - M_PI / 2);
            camera.y += frameDelta * speedModifier * sin(camera.angle - M_PI / 2);
        }

        if (moveRight) {
            camera.x += frameDelta * speedModifier * cos(camera.angle + M_PI / 2);
            camera.y += frameDelta * speedModifier * sin(camera.angle + M_PI / 2);
        }
    }
}