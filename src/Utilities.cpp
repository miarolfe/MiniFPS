#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <dirent.h>

#include "Utilities.h"

namespace MiniFPS {
    bool InitializeSDL() {
        bool successfulInitialization = true;

        if (SDL_Init((SDL_INIT_VIDEO | SDL_INIT_AUDIO)) < 0) {
            successfulInitialization = false;
        }

        return successfulInitialization;
    }

    bool InitializeWindowAndRenderer(SDL_Window** window, SDL_Renderer** renderer, size_t screenWidth,
                                     size_t screenHeight, bool vSync) {
        bool successfulInitialization = true;

        *window = SDL_CreateWindow("MiniFPS",
                                   SDL_WINDOWPOS_CENTERED,
                                   SDL_WINDOWPOS_CENTERED,
                                   screenWidth,
                                   screenHeight,
                                   SDL_WINDOW_SHOWN);

        if (*window == nullptr) {
            successfulInitialization = false;
        }

        if (vSync) {
            *renderer = SDL_CreateRenderer(*window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
        } else {
            *renderer = SDL_CreateRenderer(*window, -1, SDL_RENDERER_ACCELERATED);
        }

        if (*renderer == nullptr) {
            successfulInitialization = false;
        }

        // Capture mouse cursor and enable relative mouse mode
        SDL_SetWindowGrab(*window, SDL_TRUE);

        return successfulInitialization;
    }

    bool InitializeSDLImage() {
        bool successful_initialization = true;

        if (!IMG_Init(IMG_INIT_PNG)) {
            successful_initialization = false;
        }

        return successful_initialization;
    }

    bool InitializeSDLMixer() {
        bool successfulInitialization = true;

        if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) != 0) {
            successfulInitialization = false;
        }

        return successfulInitialization;
    }

    bool InitializeSDLTTF() {
        bool successfulInitialization = true;

        if (TTF_Init() != 0) {
            successfulInitialization = false;
        }

        return successfulInitialization;
    }

    void Quit(SDL_Window* window, SDL_Renderer* renderer) {
        SDL_DestroyWindow(window);
        SDL_DestroyRenderer(renderer);
        SDL_Quit();
        IMG_Quit();
        Mix_CloseAudio();
        TTF_Quit();
    }

    std::string GetFramesPerSecond(const float frameDelta) {
        return std::to_string(static_cast<int>((1.0f / frameDelta)));
    }

    float GetFrameTime(const float oldTime, const float curTime) {
        float frameTime = (curTime - oldTime) / 1000.0f;
        return frameTime;
    }

    std::string GetSDLAssetsFolderPath() {
        std::string filePath;
        const char* platform = SDL_GetPlatform();

        // strcmp returns 0 if two strings are identical
        if (strcmp(platform, "Windows") == 0) {
            filePath = "assets/";
        } else if (strcmp(platform, "Mac OS X") == 0) {
            filePath = "../Resources/";
        } else {
            std::cerr << "Invalid platform: " << platform << std::endl;
            filePath = "INVALID PLATFORM";
        }

        return filePath;
    }

    TTF_Font* LoadFont(const std::string &fontPath, int pointSize) {
        return TTF_OpenFont(fontPath.c_str(), pointSize);
    }

    void ClearFile(const std::string& fileName) {
        std::ofstream outFile(fileName, std::ofstream::out | std::ofstream::trunc);
        outFile.close();
    }

    void WriteLineToFile(const std::string& fileName, const std::string& line) {
        std::ofstream outFile(fileName, std::ios::app);
        outFile << line << std::endl;
        outFile.close();
    }

    std::vector<std::string> GetFilesInDirectory(const std::string& directoryPath) {
        std::vector<std::string> files;
        DIR* dir;
        struct dirent* entry;

        if ((dir = opendir(directoryPath.c_str())) != nullptr) {
            while ((entry = readdir(dir)) != nullptr) {
                if (entry->d_type == DT_REG) {  // regular file
                    files.emplace_back(entry->d_name);
                }
            }
            closedir(dir);
        } else {
            std::cerr << "Error opening directory: " << directoryPath << std::endl;
        }

        return files;
    }

    std::vector<std::string> GetFoldersInDirectory(const std::string& directoryPath) {
        std::vector<std::string> folders;
        DIR* dir;
        struct dirent* entry;

        if ((dir = opendir(directoryPath.c_str())) != nullptr) {
            while ((entry = readdir(dir)) != nullptr) {
                if (entry->d_type == DT_DIR) {  // directory
                    std::string folderName = entry->d_name;
                    if (folderName != "." && folderName != "..") {
                        folders.push_back(folderName);
                    }
                }
            }
            closedir(dir);
        } else {
            std::cerr << "Error opening directory: " << directoryPath << std::endl;
        }

        return folders;
    }
}