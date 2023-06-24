#include <iostream>
#include <fstream>
#include <cassert>
#include <cmath>
#include <vector>
#include <map>

#include "Level.h"

namespace MiniFPS {
    Level::Level(const std::string& filePath) {
        matrix = nullptr;
        w = -1;
        h = -1;
        Load(filePath);
    }

    short Level::Get(const int x, const int y) {
        assert(x >= 0);
        assert(x < w);
        assert(y >= 0);
        assert(y < h);
        return matrix[y][x];
    }

    bool Level::HasCollided(const float x, const float y) {
        bool collided = false;

        // Check if out-of-bounds
        if (x < collisionThreshold || x > w - collisionThreshold || y < collisionThreshold ||
            y > h - collisionThreshold)
            return true;

        const int roundedX = static_cast<int>(roundf(x));
        const int roundedY = static_cast<int>(roundf(y));

        for (int cellX = roundedX - 1; cellX <= roundedX + 1; cellX++) {
            for (int cellY = roundedY - 1; cellY <= roundedY + 1; cellY++) {
                if (!(cellX < 0 || cellX >= w || cellY < 0 || cellY >= h)) {
                    if (Get(cellX, cellY) != 0) {
                        if (x >= static_cast<double>(cellX) - collisionThreshold &&
                            x <= static_cast<double>(cellX) + 1 + collisionThreshold &&
                            y >= static_cast<double>(cellY) - collisionThreshold &&
                            y <= static_cast<double>(cellY) + 1 + collisionThreshold) {
                            collided = true;
                        }
                    }
                }

                if (collided) {
                    break;
                }
            }

            if (collided) {
                break;
            }
        }

        return collided;
    }

    bool Level::IsPositionValid(float x, float y) {
        return (x >= 0 && x < w && y >= 0 && y < h);
    }

    void Level::Print() {
        for (int i = 0; i < w; i++) {
            for (int j = 0; j < h; j++) {
                if (Get(j, i) == 0) {
                    std::cout << "  ";
                } else {
                    std::cout << "* ";
                }
            }
            std::cout << std::endl;
        }
    }

    void Level::Load(const std::string &filePath) {
        std::ifstream infile(filePath);
        if (!infile) {
            std::cerr << "Error opening level file for reading: " << filePath << std::endl;
        }

        infile >> w >> h;

        matrix = new short* [h];

        for (int i = 0; i < h; i++) {
            matrix[i] = new short[w];
        }

        for (int i = 0; i < h; i++) {
            for (int j = 0; j < w; j++) {
                infile >> matrix[i][j];
            }
        }

        while (infile.peek() != EOF) {
            short id;
            infile >> id;
            std::string textureName;
            infile >> textureName;
            textureIdMap[id] = textureName;
        }

        infile.close();
    }
}
