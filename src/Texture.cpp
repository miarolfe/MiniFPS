#include <iostream>

#include "Texture.h"

namespace MiniFPS {
    Texture::Texture() {

    }

    Texture::Texture(std::string name, std::string filePath) {
        this->name = name;

        SDL_Surface* tempTextureSurface = IMG_Load(filePath.c_str());
        tempTextureSurface = SDL_ConvertSurfaceFormat(tempTextureSurface, SDL_PIXELFORMAT_ARGB8888, 0);

        size = tempTextureSurface->w;
        buffer = new Uint32* [size];
        for (int row = 0; row < size; row++) {
            buffer[row] = new Uint32[size];
        }

        Uint32* pixels = (Uint32*) tempTextureSurface->pixels;

        for (int row = 0; row < size; row++) {
            for (int col = 0; col < size; col++) {
                buffer[row][col] = pixels[row * size + col];
            }
        }

        SDL_FreeSurface(tempTextureSurface);
    }
}
