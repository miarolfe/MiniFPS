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

        /**
         * Constructs a Level object by loading a level from a file.
         * @param filePath The path to the level file, in .lvl format.
         */
        explicit Level(const std::string& filePath);

        /**
         * Gets the texture ID of the cell at a position in the level. Does not check if cell position is valid.
         * @param point The cell's position in the level.
         * @return The texture ID of the cell.
         */
        short Get (IntPoint point);

        /**
         * Checks if the cell at position (x, y) in the level is occupied by a wall.
         * @param point The cell's position in the level.
         * @return The occupation status of the cell.
         */
        bool HasCollided(FloatPoint point);

        /**
         * Checks if the position (x, y) is a valid position in the level.
         * @param point The cell's position in the level.
         * @return Whether the position is valid.
         */
        bool IsPositionValid(FloatPoint point);

        /**
         * Prints a birds-eye view outline of the level to std::cout.
         */
        void Print();

        /**
         * Load a level from a level file in .lvl format.
         * @param filePath The path to load the level file from, in .lvl format.
         */
        void Load(const std::string& filePath);
    };
}