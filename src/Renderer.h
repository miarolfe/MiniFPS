#pragma once

#include <SDL.h>
#include <SDL_image.h>

#include "Camera.h"
#include "Level.h"
#include "Menu.h"

struct Texture {
    Texture();
    Texture(std::string name, std::string filePath);

    string name;
    Uint32** buffer;
    size_t size;
};

void SetPixel(void* pixels, int pitch, Uint32 color, int x, int y);

void DrawCeiling(Camera camera, int pitch, void* pixels);

void DrawFloor(Camera camera, int pitch, void* pixels);

Texture GetTexBuffer(Uint32 cellColor, Texture* textures, size_t numTextures);

void DrawText(SDL_Renderer* sdlRenderer, SDL_Texture* frameTexture, const std::string &text, Font font, SDL_Rect destRect);

void Draw(SDL_Renderer* renderer, Player player, std::map<std::string, Texture> textures, SDL_Texture* streamingFrameTexture, SDL_Texture* renderFrameTexture);

void DrawMainMenu(const Settings& settings, SDL_Renderer* renderer, const Font& font, Camera camera, SDL_Texture* streamingFrameTexture, SDL_Texture* renderFrameTexture);
