#pragma once
#ifndef MINI_FPS_UTILITIES_H
#define MINI_FPS_UTILITIES_H

const float PI_180 = (1.0 / 180.0) * M_PI;

bool InitializeSDL();

bool
InitializeWindowAndRenderer(SDL_Window** window, SDL_Renderer** renderer, size_t screenWidth, size_t screenHeight, bool vSync);

bool InitializeSDLImage();

void Quit(SDL_Window* window, SDL_Renderer* renderer);

std::string GetFramesPerSecond(double oldTime, double curTime);

double GetFrameTime(double oldTime, double curTime);

std::string GetSDLAssetsFolderPath();

std::string GetMiscAssetsFolderPath();

SDL_PixelFormatEnum GetPlatformPixelFormatEnum();

bool LoadTextureToBuffer(Uint32*** buffer, size_t& size, std::string assetsFolderPath, std::string textureFilePath);

int Clamp(int value, int min, int max);

void ClearFile(std::string fileName);

void WriteLineToFile(std::string fileName, std::string line);

#endif //MINI_FPS_UTILITIES_H