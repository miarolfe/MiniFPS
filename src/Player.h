#pragma once

#include "AudioHandler.h"
#include "Camera.h"
#include "Common.h"
#include "Level.h"
#include "Settings.h"
#include "Texture.h"

namespace MiniFPS
{
    // Forward declaration
    class Enemy;

    const float MAX_SHOT_DISTANCE = 0.2f;
    const float JUST_FIRED_TIME = 0.05f;

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

        Player(Level* level, const Settings& settings, const Vec2Int& cameraResolution);

        Level* m_level;
        Camera m_camera;
        InputState m_inputState;
        Texture m_baseWeaponTexture;
        Texture m_muzzleFlashWeaponTexture;

        int m_currentHealth = MAX_HEALTH;
        int m_currentAmmo = MAG_SIZE;

        bool m_reloading = false;
        bool m_justFired = false;
        float m_reloadTimer = 0.0f;
        float m_justFiredTimer = 0.0f;

        void UpdateInputState();

        void Move(float frameDelta, float speedModifier);

        void Rotate(float frameDelta, float rotationModifier);

        void Update(float frameDelta, float speedModifier, float rotationModifier);

        bool GameHasEnded() const;

        bool InMainMenu() const;

        bool CanShoot();

        bool Shoot(std::vector<Enemy>& enemies, float wallDistance);

        void Reload();

    public:
        static const int MAX_HEALTH = 5;
        static const int MAG_SIZE = 12;
        constexpr static const float RELOAD_TIME = 2.0f;
    };
}