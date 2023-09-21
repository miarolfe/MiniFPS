#pragma once

#include "Common.h"
#include "Point.h"

namespace MiniFPS {
    const float collisionThreshold = 0.05;

    struct Level {
    private:
        short** matrix; // 2D array representing the level as a grid of texture IDs, where 0 is empty space
        int w; // Width of the level
        int h; // Height of the level

    public:
        // TODO: Abstract this out
        std::map<short, std::string> textureIdMap; // Map of texture IDs to their names

        Level();


        explicit Level(const std::string& filePath);
        short Get (IntPoint point);
        bool HasCollided(FloatPoint point);
        bool IsPositionValid(FloatPoint point);
        void Print();
        void Load(const std::string& filePath);
    };
}