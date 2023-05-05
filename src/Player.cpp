// <iostream> needs to be here, or it doesn't compile, complaining about memset_pattern4
#include <iostream>
#include <cmath>

#include <SDL.h>

#include "Camera.h"
#include "Player.h"
#include "Settings.h"
#include "Utilities.h"

void GetInputState(bool &gameIsRunning, bool &moveLeft, bool &moveRight, bool &moveForward,
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

void Move(Camera &camera, float frameDelta, float speedModifier, bool moveLeft, bool moveRight,
          bool moveForward, bool moveBack) {

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

Player::Player(Level* level, Settings settings) {
    this->level = level;
    inputState = InputState();
    camera = Camera(settings.playerStartX, settings.playerStartY, settings.playerStartAngle,
                    settings.fieldOfView * PI_180, settings.screenWidth,
                    settings.screenHeight, settings.renderRayIncrement,
                    settings.renderDistance, settings.playerDistanceToProjectionPlane);
}

void Player::UpdateInputState() {
    inputState.mouseX = 0;
    inputState.mouseY = 0;

    SDL_Event event;

    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            inputState.endGame = true;
        }

        if (event.type == SDL_MOUSEMOTION) {
            inputState.mouseX = event.motion.xrel;
            inputState.mouseY = event.motion.yrel;
        }
    }

    const Uint8* currentKeyStates = SDL_GetKeyboardState(nullptr);

    if (currentKeyStates[SDL_SCANCODE_ESCAPE]) {
        inputState.endGame = true;
    }

    if (currentKeyStates[SDL_SCANCODE_W]) {
        inputState.moveForward = true;
    } else {
        inputState.moveForward = false;
    }

    if (currentKeyStates[SDL_SCANCODE_S]) {
        inputState.moveBack = true;
    } else {
        inputState.moveBack = false;
    }

    if (currentKeyStates[SDL_SCANCODE_A]) {
        inputState.moveLeft = true;
    } else {
        inputState.moveLeft = false;
    }

    if (currentKeyStates[SDL_SCANCODE_D]) {
        inputState.moveRight = true;
    } else {
        inputState.moveRight = false;
    }
}

void Player::Move(float frameDelta, float speedModifier) {
    float prevX = camera.x;
    float prevY = camera.y;

    if (inputState.moveForward != inputState.moveBack) {
        if (inputState.moveForward) {
            camera.x += frameDelta * speedModifier * cos(camera.angle);
            camera.y += frameDelta * speedModifier * sin(camera.angle);
        }

        if (inputState.moveBack) {
            camera.x -= frameDelta * speedModifier * cos(camera.angle);
            camera.y -= frameDelta * speedModifier * sin(camera.angle);
        }
    }

    if (inputState.moveLeft != inputState.moveRight) {
        if (inputState.moveLeft) {
            camera.x += frameDelta * speedModifier * cos(camera.angle - M_PI / 2);
            camera.y += frameDelta * speedModifier * sin(camera.angle - M_PI / 2);
        }

        if (inputState.moveRight) {
            camera.x += frameDelta * speedModifier * cos(camera.angle + M_PI / 2);
            camera.y += frameDelta * speedModifier * sin(camera.angle + M_PI / 2);
        }
    }

    if (level->HasCollided(camera.x, camera.y)) {
        camera.x = prevX;
        camera.y = prevY;
    }
}

void Player::Rotate(float frameDelta, float rotationModifier) {
    camera.angle += inputState.mouseX * frameDelta * rotationModifier;
}

bool Player::GameHasEnded() {
    return inputState.endGame;
}

void Player::Update(float frameDelta, float speedModifier, float rotationModifier) {
    UpdateInputState();
    Move(frameDelta, speedModifier);
    Rotate(frameDelta, rotationModifier);
}

InputState::InputState() {
    moveLeft = false;
    moveRight = false;
    moveForward = false;
    moveBack = false;
    endGame = false;
    mouseX = 0;
    mouseY = 0;
}
