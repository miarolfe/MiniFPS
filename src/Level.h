#pragma once

#include <vector>
#include <SDL.h>

#include "Settings.h"

namespace MiniFPS {
    struct Level {
        short** matrix;
        size_t w, h;
        std::map<short, std::string> textureIdMap;

        explicit Level(std::string filePath);
        short Get (int x, int y);
        bool HasCollided(float x, float y);
        void Print();
        void SaveToLVL(const std::string& filePath);
        void LoadFromLVL(std::string filePath);
    };
}