//
// Created by Peter Rolfe on 19/02/2023.
//

#pragma once
#ifndef MINI_FPS_LEVEL_H
#define MINI_FPS_LEVEL_H
#include <vector>
#include <SDL.h>

struct Level {
    Uint32** matrix;
    size_t w, h;
    SDL_PixelFormat pixelFormat;
    explicit Level(const char* filePath);
    Uint32 Get (size_t x, size_t y);
    bool HasCollided(float x, float y);
    void Print();
    bool PNGToTwoDimensionalArray(const char *filePath);
};

#endif //MINI_FPS_LEVEL_H
