#pragma once

#include <SDL.h>
#include <SDL_image.h>

#include "Camera.h"
#include "Level.h"
#include "Menu.h"

void SetPixel(void* pixels, int pitch, Uint32 color, int x, int y);

void DrawText(SDL_Renderer* sdlRenderer, SDL_Texture* frameTexture, const std::string &text, Font font, SDL_Rect destRect);

void Draw(SDL_Renderer* renderer, Player player, Uint32**** texBuffers, size_t numTexBuffers, size_t texSize,
          SDL_Texture* streamingFrameTexture, SDL_Texture* renderFrameTexture);

void DrawMainMenu(SDL_Renderer* renderer, Font font, Camera camera, SDL_Texture* streamingFrameTexture, SDL_Texture* renderFrameTexture);
