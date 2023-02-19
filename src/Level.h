//
// Created by Peter Rolfe on 19/02/2023.
//

#ifndef MINI_FPS_LEVEL_H
#define MINI_FPS_LEVEL_H
#include <vector>
#include <SDL.h>

struct Level {
    std::vector<std::vector<Uint32>> matrix;
    size_t w, h;
    explicit Level(const char* filePath);
    Uint32 get (size_t x, size_t y);
    void print();
};

bool png_to_matrix(std::vector<std::vector<Uint32>>& matrix, const char *filePath);

#endif //MINI_FPS_LEVEL_H
