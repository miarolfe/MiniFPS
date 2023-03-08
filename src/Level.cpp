#include <iostream>
#include <cmath>
#include <vector>
#include <string>
#include <SDL_image.h>

#include "Level.h"
#include "Color.h"

bool Level::png_to_two_dimensional_array(const char* filePath) {
    bool successfulLoadAndConversion = true;

    SDL_Surface* surface;
    surface = IMG_Load(filePath);
    if (surface == NULL) {
        successfulLoadAndConversion = false;
    }

    pixelFormat = *SDL_AllocFormat(surface->format->format);

    w = surface->w;
    h = surface->h;

    matrix = new Uint32* [surface->h];

    for (size_t i = 0; i < surface->h; i++) {
        matrix[i] = new Uint32[surface->w];
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

    if (!png_to_two_dimensional_array(filePath)) {
        std::cerr << "Image could not be loaded and/or converted to a level" << std::endl;
    } else {
        std::cout << "Image loaded and converted" << std::endl;
    }
}

Uint32 Level::get(const size_t x, const size_t y) {
    // This can be removed once collision detection is a thing
    if (y < 0 || y >= h) {
        std::cerr << "Invalid level matrix access" << std::endl;
        return 0;
    }

    if (x < 0 || x >= w) {
        std::cerr << "Invalid level matrix access" << std::endl;
        return 0;
    }

    return matrix[y][x];
}

bool Level::has_collided(const float x, const float y) {
    bool collided = false;

    int roundedX = round(x);
    int roundedY = round(y);

    for (size_t cellX = roundedX - 1; cellX <= roundedX + 1; cellX++) {
        for (size_t cellY = roundedY - 1; cellY <= roundedY + 1; cellY++) {
            if (get(cellX, cellY) != AGBR_WHITE) {
                if (x >= cellX - 0.05 && x <= cellX + 1 + 0.05 &&
                    y >= cellY - 0.05 && y <= cellY + 1 + 0.05) {
                    collided = true;
                }
            }
        }
    }

    return collided;
}

void Level::print() {
    for (int i = 0; i < w; i++) {
        for (int j = 0; j < h; j++) {
            if (get(j, i) == AGBR_WHITE) {
                std::cout << "  ";
            } else {
                std::cout << "* ";
            }
        }
        std::cout << std::endl;
    }
}
