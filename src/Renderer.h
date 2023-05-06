#pragma once

#include <SDL.h>
#include <SDL_image.h>

#include "Camera.h"
#include "Level.h"

void SetPixel(void* pixels, int pitch, Uint32 color, int x, int y);

void Draw(SDL_Renderer* renderer, Camera camera, Level &level, Uint32**** texBuffers, size_t numTexBuffers, size_t texSize,
          SDL_Texture* frameTexture);

void DrawMainMenu(SDL_Renderer* renderer, Camera camera, SDL_Texture* frameTexture);
