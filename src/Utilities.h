#pragma once

#include <SDL_ttf.h>

const float PI_180 = (1.0 / 180.0) * M_PI;

bool InitializeSDL();

bool InitializeWindowAndRenderer(SDL_Window** window, SDL_Renderer** renderer, size_t screenWidth, size_t screenHeight, bool vSync);

bool InitializeSDLImage();

bool InitializeSDLTTF();

void Quit(SDL_Window* window, SDL_Renderer* renderer);

std::string GetFramesPerSecond(float oldTime, float curTime);

float GetFrameTime(float oldTime, float curTime);

std::string GetSDLAssetsFolderPath();

std::string GetMiscAssetsFolderPath();

SDL_PixelFormatEnum GetPlatformPixelFormatEnum();

bool LoadTextureToBuffer(Uint32*** buffer, size_t& size, std::string assetsFolderPath, std::string textureFilePath);

TTF_Font* LoadFont(const std::string &fontPath, int pointSize);

int Clamp(int value, int min, int max);

void ClearFile(std::string fileName);

void WriteLineToFile(std::string fileName, std::string line);