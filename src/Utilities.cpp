#include <iostream>
#include <fstream>
#include <string>

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include "Utilities.h"

namespace MiniFPS {
    bool InitializeSDL() {
        bool successful_initialization = true;

        if (SDL_Init(SDL_INIT_VIDEO) < 0) {
            successful_initialization = false;
        }

        return successful_initialization;
    }

    bool InitializeWindowAndRenderer(SDL_Window** window, SDL_Renderer** renderer, size_t screenWidth,
                                     size_t screenHeight, bool vSync) {
        bool successful_initialization = true;

        *window = SDL_CreateWindow("MiniFPS",
                                   SDL_WINDOWPOS_CENTERED,
                                   SDL_WINDOWPOS_CENTERED,
                                   screenWidth,
                                   screenHeight,
                                   SDL_WINDOW_SHOWN);

        if (*window == nullptr) {
            successful_initialization = false;
        }

        if (vSync) {
            *renderer = SDL_CreateRenderer(*window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
        } else {
            *renderer = SDL_CreateRenderer(*window, -1, SDL_RENDERER_ACCELERATED);
        }

        if (*renderer == nullptr) {
            successful_initialization = false;
        }

        // Capture mouse cursor and enable relative mouse mode
        SDL_SetWindowGrab(*window, SDL_TRUE);

        return successful_initialization;
    }

    bool InitializeSDLImage() {
        bool successful_initialization = true;

        if (!IMG_Init(IMG_INIT_PNG)) {
            successful_initialization = false;
        }

        return successful_initialization;
    }

    bool InitializeSDLTTF() {
        bool successful_initialization = true;

        if (TTF_Init() != 0) {
            successful_initialization = false;
        }

        return successful_initialization;
    }

    void Quit(SDL_Window* window, SDL_Renderer* renderer) {
        SDL_DestroyWindow(window);
        SDL_DestroyRenderer(renderer);
        SDL_Quit();
        IMG_Quit();
        TTF_Quit();
    }

    std::string GetFramesPerSecond(const float oldTime, const float curTime) {
        return std::to_string(static_cast<int>((1.0f / GetFrameTime(oldTime, curTime))));
    }

    float GetFrameTime(const float oldTime, const float curTime) {
        float frameTime = (curTime - oldTime) / 1000.0f;
        return frameTime;
    }
    
    std::string GetSDLAssetsFolderPath() {
        std::string file_path;
        const char* platform = SDL_GetPlatform();

        // strcmp returns 0 if two strings are identical
        if (strcmp(platform, "Windows") == 0) {
            file_path = "assets/";
        } else if (strcmp(platform, "Mac OS X") == 0) {
            file_path = "../Resources/";
        } else {
            std::cerr << "Invalid platform: " << platform << std::endl;
            file_path = "INVALID PLATFORM";
        }

        return file_path;
    }

    TTF_Font* LoadFont(const std::string &fontPath, int pointSize) {
        return TTF_OpenFont(fontPath.c_str(), pointSize);
    }

    void ClearFile(std::string fileName) {
        std::ofstream outFile(fileName, std::ofstream::out | std::ofstream::trunc);
        outFile.close();
    }

    void WriteLineToFile(std::string fileName, std::string line) {
        std::ofstream outFile(fileName, std::ios::app); // open file in append mode
        outFile << line << std::endl; // write line and append newline character
        outFile.close(); // close file
    }

    std::vector<std::string> GetFilesInDirectory(const std::string& directoryPath) {
        std::vector<std::string> files;
        DIR* dir;
        struct dirent* entry;

        if ((dir = opendir(directoryPath.c_str())) != nullptr) {
            while ((entry = readdir(dir)) != nullptr) {
                if (entry->d_type == DT_REG) {  // regular file
                    files.push_back(entry->d_name);
                }
            }
            closedir(dir);
        } else {
            std::cerr << "Error opening directory: " << directoryPath << std::endl;
        }

        return files;
    }
}