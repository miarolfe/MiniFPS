#pragma once
#ifndef MINI_FPS_PLAYER_H
#define MINI_FPS_PLAYER_H

void GetInputState(bool &gameIsRunning, bool &moveLeft, bool &moveRight, bool &moveForward,
                   bool &moveBack, int &mouseX, int &mouseY);

void Move(Camera &camera, float frameDelta, float speedModifier, bool moveLeft, bool moveRight,
          bool moveForward, bool moveBack);

#endif //MINI_FPS_PLAYER_H
