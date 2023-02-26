#include <iostream>
#include <vector>
#include <string>
#include <SDL_image.h>

#include "Level.h"
#include "Color.h"

bool png_to_matrix(std::vector<std::vector<Uint32>>& matrix, const char *filePath, SDL_PixelFormat& pixelFormat)  {
    bool successfulLoadAndConversion = true;

    SDL_Surface* surface;
    surface = IMG_Load(filePath);
    if (surface == NULL) {
        successfulLoadAndConversion = false;
    }

    pixelFormat = *SDL_AllocFormat(surface->format->format);

    matrix.resize(surface->h);

    for (size_t i = 0; i < surface->h; i++) {
        matrix[i].resize(surface->w);
    }

    int pitch = surface->pitch / sizeof(Uint32);

    Uint32* pixels = static_cast<Uint32*>(surface->pixels);

    for (int y = 0; y < surface->h; y++) {
        for (int x = 0; x < surface->w; x++) {
            Uint32 pixel = pixels[y * pitch + x];
            Uint8 r, g, b, a;
            SDL_GetRGBA(pixel, surface->format, &r, &g, &b, &a);
            matrix[y][x] = pixel;
        }
    }

    SDL_FreeSurface(surface);

    return successfulLoadAndConversion;
}

Level::Level(const char* filePath) {
    if (filePath == nullptr) {
        std::cerr << "Null file path" << std::endl;
        return;
    }

    if (!png_to_matrix(matrix, filePath, pixelFormat)) {
        std::cerr << "Image could not be loaded and/or converted to a level" << std::endl;
    } else {
        std::cout << "Image loaded and converted" << std::endl;
    }
    w = matrix.size();
    h = matrix[0].size();
}

Uint32 Level::get(const size_t x, const size_t y) {
    // This can be removed once collision detection is a thing
    if (y < 0 || y >= matrix.size()) {
        std::cerr << "Invalid level matrix access" << std::endl;
        return 0;
    }

    if (x < 0 || x >= matrix[y].size()) {
        std::cerr << "Invalid level matrix access" << std::endl;
        return 0;
    }

    return matrix[y][x];
}

void Level::print() {
    for (int i = 0; i < w; i++)
    {
        for (int j = 0; j < h; j++)
        {
            if (get(j, i) == RGBA_WHITE) {
                std::cout << "  ";
            } else {
                std::cout << "* ";
            }
        }
        std::cout << std::endl;
    }
}
