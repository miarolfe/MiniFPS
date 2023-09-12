#include "Level.h"

namespace MiniFPS {
    Level::Level() = default;

    Level::Level(const std::string& filePath) {
        matrix = nullptr;
        w = -1;
        h = -1;
        Load(filePath);
    }

    short Level::Get(IntPoint point) {
        assert(point.x >= 0);
        assert(point.x < w);
        assert(point.y >= 0);
        assert(point.y < h);
        return matrix[point.y][point.x];
    }

    bool Level::HasCollided(const FloatPoint point) {
        bool collided = false;

        // Check if out-of-bounds
        if (point.x < collisionThreshold || point.x > w - collisionThreshold || point.y < collisionThreshold ||
                point.y > h - collisionThreshold)
            return true;

        const int roundedX = static_cast<int>(roundf(point.x));
        const int roundedY = static_cast<int>(roundf(point.y));

        for (int cellX = roundedX - 1; cellX <= roundedX + 1; cellX++) {
            for (int cellY = roundedY - 1; cellY <= roundedY + 1; cellY++) {
                if (!(cellX < 0 || cellX >= w || cellY < 0 || cellY >= h)) {
                    if (Get({cellX, cellY}) != 0) {
                        if (point.x >= static_cast<double>(cellX) - collisionThreshold &&
                                point.x <= static_cast<double>(cellX) + 1 + collisionThreshold &&
                                point.y >= static_cast<double>(cellY) - collisionThreshold &&
                                point.y <= static_cast<double>(cellY) + 1 + collisionThreshold) {
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

    bool Level::IsPositionValid(const FloatPoint point) {
        return (point.x >= 0 && point.x < w && point.y >= 0 && point.y < h);
    }

    void Level::Print() {
        for (int i = 0; i < w; i++) {
            for (int j = 0; j < h; j++) {
                if (Get({j, i}) == 0) {
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
