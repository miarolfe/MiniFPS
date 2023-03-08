//
// Created by Peter Rolfe on 23/02/2023.
//

#ifndef MINI_FPS_RENDERER_H
#define MINI_FPS_RENDERER_H

#include <SDL.h>
#include <SDL_image.h>

#include "Camera.h"
#include "Level.h"

void set_pixel(void* pixels, int pitch, Uint32 color, int x, int y);

void draw(SDL_Renderer* renderer, Camera camera, Level &level, Uint32**** texBuffers, size_t numTexBuffers, size_t texSize,
          SDL_Texture* frameTexture);

#endif //MINI_FPS_RENDERER_H
