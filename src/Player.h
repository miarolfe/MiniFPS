#pragma once
#ifndef MINI_FPS_PLAYER_H
#define MINI_FPS_PLAYER_H

void get_input_state(bool &gameIsRunning, bool &moveLeft, bool &moveRight, bool &moveForward,
                     bool &moveBack, int &mouseX, int &mouseY);

void move(Camera &camera, float frameDelta, float speedModifier, bool moveLeft, bool moveRight,
          bool moveForward, bool moveBack);

#endif //MINI_FPS_PLAYER_H
