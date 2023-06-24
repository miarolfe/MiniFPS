#pragma once

#include <map>

#include <SDL.h>

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

        /**
         * Constructs a Level object by loading a level from a file.
         * @param filePath The path to the level file, in .lvl format.
         */
        explicit Level(const std::string& filePath);

        /**
         * Gets the texture ID of the cell at position (x, y) in the level. Does not check if cell position is valid.
         * @param x The x-coordinate of the cell.
         * @param y The y-coordinate of the cell.
         * @return The texture ID of the cell.
         */
        short Get (int x, int y);

        /**
         * Checks if the cell at position (x, y) in the level is occupied by a wall.
         * @param x The x-coordinate of the cell.
         * @param y The y-coordinate of the cell.
         * @return The occupation status of the cell.
         */
        bool HasCollided(float x, float y);

        /**
         * Checks if the position (x, y) is a valid position in the level.
         * @param x The x-coordinate of the cell.
         * @param y The y-coordinate of the cell.
         * @return Whether the position is valid.
         */
        bool IsPositionValid(float x, float y);

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