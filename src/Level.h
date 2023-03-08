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
    Uint32 get (size_t x, size_t y);
    bool has_collided(float x, float y);
    void print();
    bool png_to_two_dimensional_array(const char *filePath);
};

#endif //MINI_FPS_LEVEL_H
