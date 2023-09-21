#pragma once

#include "Camera.h"
#include "Common.h"
#include "Enemy.h"
#include "Level.h"
#include "Settings.h"
#include "Texture.h"

namespace MiniFPS {
    struct InputState {
        InputState();

        bool moveLeft, moveRight, moveForward, moveBack, endGame, inMainMenu, leftMouseButtonPressed,
        rightMouseButtonPressed;
        int mousePosX, mousePosY, mouseMotionX, mouseMotionY;
    };

    class Player {
    public:
        Player();
        Player(Level* level, const Settings& settings);

        Level* level;
        Camera camera;
        InputState inputState;
        Texture weaponTexture;

        void UpdateInputState();
        void Move(float frameDelta, float speedModifier);
        void Rotate(float frameDelta, float rotationModifier);
        void Update(float frameDelta, float speedModifier, float rotationModifier);
        bool GameHasEnded() const;
        bool InMainMenu() const;
        bool Shoot(std::vector<Enemy>& enemies);
    };
}