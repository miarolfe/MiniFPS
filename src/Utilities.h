#pragma once
#ifndef MINI_FPS_UTILITIES_H
#define MINI_FPS_UTILITIES_H

const float PI_180 = (1.0 / 180.0) * M_PI;

bool initialize_sdl();

bool
initialize_window_and_renderer(SDL_Window** window, SDL_Renderer** renderer, size_t screenWidth, size_t screenHeight, bool vSync);

bool initialize_sdl_image();

void quit(SDL_Window* window);

std::string frames_per_second(double oldTime, double curTime);

double frame_time(double oldTime, double curTime);

std::string assets_folder_path();

bool load_texture_to_buffer(Uint32*** buffer, size_t& size, std::string assetsFolderPath, std::string textureFilePath);

int clamp(int value, int min, int max);

#endif //MINI_FPS_UTILITIES_H