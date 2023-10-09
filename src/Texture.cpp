#include "Texture.h"

namespace MiniFPS
{
    Texture::Texture()
    {
        buffer = nullptr;
        size = -1;
    }

    Texture::Texture(const std::string &name, const std::string &filePath)
    {
        this->name = name;

        SDL_Surface* tempTextureSurface = IMG_Load(filePath.c_str());
        tempTextureSurface = SDL_ConvertSurfaceFormat(tempTextureSurface, SDL_PIXELFORMAT_ARGB8888, 0);

        size = tempTextureSurface->w;
        buffer = new Color* [size];
        for (int row = 0; row < size; row++)
        {
            buffer[row] = new Color[size];
        }

        uint32_t* pixels = (uint32_t*) tempTextureSurface->pixels;

        for (int row = 0; row < size; row++)
        {
            for (int col = 0; col < size; col++)
            {
                buffer[row][col].argb = pixels[row * size + col];
            }
        }

        SDL_FreeSurface(tempTextureSurface);
    }
}
