#pragma once

#include "Audio.h"
#include "Camera.h"
#include "Common.h"
#include "Enemy.h"
#include "Level.h"
#include "Settings.h"
#include "Texture.h"

namespace MiniFPS
{
    const float MAX_SHOT_DISTANCE = 0.4f;

    struct InputState
    {
        InputState();

        bool moveLeft, moveRight, moveForward, moveBack, endGame, inMainMenu, leftMouseButtonPressed,
                rightMouseButtonPressed;
        int mousePosX, mousePosY, mouseMotionX, mouseMotionY;
    };

    class Player
    {
    public:
        Player();

        Player(Level* level, const Settings &settings);

        Level* m_level;
        Camera m_camera;
        InputState m_inputState;
        Texture m_weaponTexture;

        int m_currentHealth = MAX_HEALTH;
        int m_currentAmmo = MAG_SIZE;

        bool m_reloading = false;
        float m_reloadTimer;

        void UpdateInputState();

        void Move(float frameDelta, float speedModifier);

        void Rotate(float frameDelta, float rotationModifier);

        void Update(float frameDelta, float speedModifier, float rotationModifier);

        bool GameHasEnded() const;

        bool InMainMenu() const;

        bool CanShoot();

        bool Shoot(std::vector<Enemy> &enemies, float wallDistance, Audio &audio);

        void Reload(Audio &audio);

    public:
        static const int MAX_HEALTH = 5;
        static const int MAG_SIZE = 12;
        constexpr static const float RELOAD_TIME = 2.0f;
    };
}