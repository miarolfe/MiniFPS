#pragma once

#include <vector>
#include <SDL.h>

#include "Settings.h"

struct Level {
    Uint32** matrix;
    size_t w, h;
    std::map<char, std::string> textureMap;

    explicit Level(std::string filePath);
    Uint32 Get (int x, int y);
    bool HasCollided(float x, float y);
    void Print();
    void SaveToLVL(const std::string& filePath);
    void LoadFromLVL(std::string filePath);
};