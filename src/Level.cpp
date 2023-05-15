#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>

#include "Level.h"
#include "Color.h"
#include "Utilities.h"

Level::Level(std::string filePath) {
    LoadFromLVL(filePath);
}

// Gets the color of pixel at cell (x, y) in level
short Level::Get(int x, int y) {
    return matrix[y][x];
}

// Returns whether a point is inside a wall cell in the level
bool Level::HasCollided(const float x, const float y) {
    bool collided = false;

    int roundedX = static_cast<int>(roundf(x));
    int roundedY = static_cast<int>(roundf(y));

    if (x < 0.05 || x > w - 0.05 || y < 0.05 || y > h - 0.05) return true;

    for (int cellX = roundedX - 1; cellX <= roundedX + 1; cellX++) {
        for (int cellY = roundedY - 1; cellY <= roundedY + 1; cellY++) {
            if (cellX < 0 || cellX >= w || cellY < 0 || cellY >= h) {
                // std::cerr << "Invalid index" << std::endl;
            } else {
                if (Get(cellX, cellY) != ARGB_WHITE) {
                    if (x >= static_cast<double>(cellX) - 0.05 && x <= static_cast<double>(cellX) + 1 + 0.05 &&
                        y >= static_cast<double>(cellY) - 0.05 && y <= static_cast<double>(cellY) + 1 + 0.05) {
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

// Prints the level's wall cells to cout
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

void Level::SaveToLVL(const std::string& filePath) {
    std::ofstream outfile(filePath);
    if (!outfile) {
        std::cerr << "Error opening file for writing: " << filePath << std::endl;
    }

    outfile << h << " " << w << std::endl;
    for (size_t i = 0; i < h; ++i) {
        for (size_t j = 0; j < w; ++j) {
            outfile << matrix[i][j] << (j < h - 1 ? " " : "");
        }
        outfile << std::endl;
    }

    for (std::pair<short, std::string> pair : textureMap) {
        outfile << pair.first << " " << pair.second << std::endl;
    }

    outfile.close();
}

void Level::LoadFromLVL(std::string filePath) {
    std::ifstream infile(filePath);
    if (!infile) {
        std::cerr << "Error opening level file for reading: " << filePath << std::endl;
    } else {
        std::cout << "Loaded level file: " << filePath << std::endl;
    }

    infile >> w >> h;

    matrix = new short* [h];

    for (size_t i = 0; i < h; i++) {
        matrix[i] = new short[w];
    }


    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            infile >> matrix[i][j];
        }
    }

    while (infile.peek()!=EOF) {
        short id;
        infile >> id;
        std::string textureName;
        infile >> textureName;
        textureMap[id] = textureName;
    }

    infile.close();
}
