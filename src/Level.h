#pragma once

#include "Common.h"
#include "LogHandler.h"
#include "Vector.h"

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
        std::map<short, string> m_textureIdMap; // Map of texture IDs to their names
        std::vector<std::pair<short, Vec2>> m_enemySpawnLocations;

        Level();

        explicit Level(const string& filePath);

        short Get(const Vec2Int& point);

        bool HasCollided(const Vec2& point);

        bool IsPositionValid(const Vec2& point);

        bool IsPositionValid(const Vec2Int& point);

        void Load(const string& filePath);

        int GetWidth();

        int GetHeight();
    };
}