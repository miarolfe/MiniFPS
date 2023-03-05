#ifndef MINI_FPS_UTILITIES_H
#define MINI_FPS_UTILITIES_H

bool initialize_sdl();
bool initialize_window_and_renderer(SDL_Window **window, SDL_Renderer **renderer, size_t SCREEN_WIDTH, size_t SCREEN_HEIGHT);
bool initialize_sdl_image();
void quit(SDL_Window *window);
std::string frames_per_second(double oldTime, double curTime);
std::string assets_file_path();

#endif //MINI_FPS_UTILITIES_H