#pragma once

#include "Common.h"
#include "Vector.h"
#include "Point.h"

namespace MiniFPS
{
    const float collisionThreshold = 0.05;

    class Level
    {
    private:
        short** m_matrix = nullptr; // 2D array representing the level as a grid of texture IDs, where 0 is empty space
        int m_width = -1; // Width of the level
        int m_height = -1; // Height of the level

    public:
        // TODO: Abstract this out
        std::map<short, std::string> m_textureIdMap; // Map of texture IDs to their names
        std::vector<std::pair<short, Vec2>> m_enemySpawnLocations;

        Level();

        explicit Level(const std::string& filePath);

        short Get(IntPoint point);

        bool HasCollided(FloatPoint point);

        bool IsPositionValid(FloatPoint point);

        void Print();

        void Load(const std::string& filePath);
    };
}