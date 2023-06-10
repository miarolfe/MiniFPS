#pragma once

#include "Camera.h"
#include "Level.h"
#include "Settings.h"

namespace MiniFPS {
    struct InputState {
        InputState();

        bool moveLeft, moveRight, moveForward, moveBack, endGame, inMainMenu;
        int mouseX, mouseY;
    };

    struct Player {
        Player();
        Player(Level* level, const Settings& settings);

        Level* level;
        Camera camera;
        InputState inputState;

        /**
         * Gets input from the player.
         */
        void UpdateInputState();

        /**
         * Applies input from the frame to the player and moves them.
         * @param frameDelta The time between frames in seconds.
         * @param speedModifier How fast the player moves.
         */
        void Move(float frameDelta, float speedModifier);

        /**
         * Applies input from the frame to the player and turns them.
         * @param frameDelta The time between frames in seconds.
         * @param rotationModifier How fast the player turns.
         */
        void Rotate(float frameDelta, float rotationModifier);

        /**
         *
         * @param frameDelta The time between frames in seconds.
         * @param speedModifier How fast the player moves.
         * @param rotationModifier How fast the player turns.
         */
        void Update(float frameDelta, float speedModifier, float rotationModifier);

        /**
         * Checks whether the game has ended.
         * @return Whether the game has ended.
         */
        bool GameHasEnded() const;

        /**
         * Checks if the game is on the main menu.
         * @return Whether the game is on the main menu.
         */
        bool InMainMenu() const;
    };
}