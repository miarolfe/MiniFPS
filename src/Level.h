#pragma once

#include <vector>
#include <SDL.h>

struct Level {
    Uint32** matrix;
    size_t w, h;
    SDL_PixelFormat pixelFormat;
    // explicit Level(const char* filePath);
    explicit Level(std::string filePath);
    Uint32 Get (int x, int y);
    bool HasCollided(float x, float y);
    void Print();
    // bool PNGToTwoDimensionalArray(const char *filePath);

    // New level format
    void SaveToLVL(const std::string& filePath);
    void LoadFromLVL(std::string filePath);
};