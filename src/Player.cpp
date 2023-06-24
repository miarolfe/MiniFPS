#include <iostream> // Won't compile without this (memset_pattern4)
#include <cmath>

#include <SDL.h>

#include "Camera.h"
#include "Player.h"
#include "Settings.h"
#include "Utilities.h"

namespace MiniFPS {
    Player::Player(Level* level, const Settings& settings) {
        this->level = level;
        inputState = InputState();
        camera = Camera(settings.playerStartX, settings.playerStartY, settings.playerStartAngle,
                        settings.fieldOfView * PI_180, settings.screenWidth,
                        settings.screenHeight, settings.renderRayIncrement,
                        settings.renderDistance,
                        settings.playerDistanceToProjectionPlane);
    }

    Player::Player() {
        this->level = nullptr;
    }

    void Player::UpdateInputState() {
        inputState.mouseMotionX = 0;
        inputState.mouseMotionY = 0;
        inputState.endGame = false;
        inputState.leftMouseButtonPressed = false;
        inputState.rightMouseButtonPressed = false;

        SDL_Event event;

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                inputState.endGame = true;
            }

            if (event.type == SDL_MOUSEMOTION) {
                inputState.mouseMotionX = event.motion.xrel;
                inputState.mouseMotionY = event.motion.yrel;
                inputState.mousePosX = event.button.x;
                inputState.mousePosY = event.button.y;
            }

            if (event.type == SDL_MOUSEBUTTONDOWN) {
                if (event.button.button == SDL_BUTTON_LEFT) {
                    inputState.leftMouseButtonPressed = true;
                } else if (event.button.button == SDL_BUTTON_RIGHT) {
                    inputState.rightMouseButtonPressed = true;
                }
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
        if (frameDelta != 0 && speedModifier != 0) {
            const float prevX = camera.pos.x;
            const float prevY = camera.pos.y;

            if (inputState.moveForward != inputState.moveBack) {
                if (inputState.moveForward) {
                    camera.pos.x += frameDelta * speedModifier * cos(camera.angle);
                    camera.pos.y += frameDelta * speedModifier * sin(camera.angle);
                }

                if (inputState.moveBack) {
                    camera.pos.x -= frameDelta * speedModifier * cos(camera.angle);
                    camera.pos.y -= frameDelta * speedModifier * sin(camera.angle);
                }
            }

            if (inputState.moveLeft != inputState.moveRight) {
                if (inputState.moveLeft) {
                    camera.pos.x += frameDelta * speedModifier * cos(camera.angle - M_PI / 2);
                    camera.pos.y += frameDelta * speedModifier * sin(camera.angle - M_PI / 2);
                }

                if (inputState.moveRight) {
                    camera.pos.x += frameDelta * speedModifier * cos(camera.angle + M_PI / 2);
                    camera.pos.y += frameDelta * speedModifier * sin(camera.angle + M_PI / 2);
                }
            }

            if (level->HasCollided({camera.pos.x, camera.pos.y})) {
                camera.pos.x = prevX;
                camera.pos.y = prevY;
            }
        }
    }

    void Player::Rotate(float frameDelta, float rotationModifier) {
        camera.angle += inputState.mouseMotionX * frameDelta * rotationModifier;
    }

    bool Player::GameHasEnded() const {
        return inputState.endGame;
    }

    bool Player::InMainMenu() const {
        return inputState.inMainMenu;
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
        inMainMenu = false;
        leftMouseButtonPressed = false;
        rightMouseButtonPressed = false;
        mousePosX = 0;
        mousePosY = 0;
    }
}