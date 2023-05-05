#pragma once

#include "Camera.h"
#include "Level.h"
#include "Settings.h"

struct InputState {
    InputState();

    bool moveLeft, moveRight, moveForward, moveBack, endGame;
    int mouseX, mouseY;
};

struct Player {
    Player(Level* level, Settings settings);

    Level* level;
    Camera camera;
    InputState inputState;

    void UpdateInputState();
    void Move(float frameDelta, float speedModifier);
    void Rotate(float frameDelta, float rotationModifier);
    void Update(float frameDelta, float speedModifier, float rotationModifier);
    bool GameHasEnded();
};

void GetInputState(bool &gameIsRunning, bool &moveLeft, bool &moveRight, bool &moveForward,
                   bool &moveBack, int &mouseX, int &mouseY);

void Move(Camera &camera, float frameDelta, float speedModifier, bool moveLeft, bool moveRight,
          bool moveForward, bool moveBack);
