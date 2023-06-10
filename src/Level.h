#pragma once

#include <vector>
#include <SDL.h>

#include "Settings.h"

namespace MiniFPS {
    struct Level {
    private:
        short** matrix; // 2D array representing the level as a grid of texture IDs, where 0 is empty space
        size_t w; // Width of the level
        size_t h; // Height of the level
    public:
        // TODO: Abstract this out
        std::map<short, std::string> textureIdMap; // Map of texture IDs to their names

        /**
         * Constructs a Level object by loading a level from a file.
         * @param filePath The path to the level file, in .lvl format.
         */
        explicit Level(const std::string& filePath);

        /**
         * Gets the texture ID of the cell at position (x, y) in the level.
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
         * Prints a birds-eye view outline of the level to std::cout.
         */
        void Print();

        /**
         * Load a level from a level file in .lvl format.
         * @param filePath The path to load the level file from, in .lvl format.
         */
        void Load(std::string filePath);
    };
}