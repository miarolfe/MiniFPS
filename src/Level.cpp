#include <iostream>
#include <vector>
#include <string>
#include <SDL_image.h>

bool png_to_matrix(std::vector<std::vector<Uint32>>& matrix, const char *filePath)  {
    bool successfulLoadAndConversion = true;

    SDL_Surface* surface;
    surface = IMG_Load(filePath);
    if (surface == NULL) {
        successfulLoadAndConversion = false;
    }

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
//            std::cout << static_cast<int>(r) << ","
//            << static_cast<int>(g) << ","
//            << static_cast<int>(b) << ","
//            << static_cast<int>(a) << std::endl;
        }
    }

    return successfulLoadAndConversion;
}