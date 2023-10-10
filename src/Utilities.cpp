#include "Utilities.h"
#include "AudioHandler.h"

namespace MiniFPS
{
    bool InitWindow(SDL_Window** window, int screenWidth, int screenHeight)
    {
        bool successfulInitialization = true;

        *window = SDL_CreateWindow("MiniFPS",
                                   SDL_WINDOWPOS_CENTERED,
                                   SDL_WINDOWPOS_CENTERED,
                                   screenWidth,
                                   screenHeight,
                                   SDL_WINDOW_SHOWN);

        // Capture mouse cursor and enable relative mouse mode
        SDL_SetWindowGrab(*window, SDL_TRUE);

        if (*window == nullptr)
        {
            successfulInitialization = false;
        }

        return successfulInitialization;
    }

    bool InitRenderer(SDL_Window* window, SDL_Renderer** renderer, bool vSync)
    {
        bool successfulInit = true;

        if (vSync)
        {
            *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
        }
        else
        {
            *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
        }

        if (*renderer == nullptr)
        {
            successfulInit = false;
        }

        return successfulInit;
    }

    bool InitSDL()
    {
        bool successfulInitialization = true;

        if (SDL_Init((SDL_INIT_VIDEO | SDL_INIT_AUDIO)) < 0)
        {
            successfulInitialization = false;
        }

        return successfulInitialization;
    }

    void ShutdownSDL()
    {
        SDL_QuitSubSystem(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
        SDL_Quit();
    }

    bool InitSDLImage()
    {
        bool successful_initialization = true;

        if (!IMG_Init(IMG_INIT_PNG))
        {
            successful_initialization = false;
        }

        return successful_initialization;
    }

    void ShutdownSDLImage()
    {
        IMG_Quit();
    }

    bool InitSDLMixer()
    {
        bool successfulInitialization = true;

        if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) != 0)
        {
            successfulInitialization = false;
        }

        return successfulInitialization;
    }

    void ShutdownSDLMixer()
    {
        Mix_CloseAudio();
        Mix_Quit();
    }

    bool InitSDLTTF()
    {
        bool successfulInitialization = true;

        if (TTF_Init() != 0)
        {
            successfulInitialization = false;
        }

        return successfulInitialization;
    }

    void ShutdownSDLTTF()
    {
        TTF_Quit();
    }

    void FreeResources(Renderer renderer, AudioHandler audio, FontManager fontManager)
    {
        renderer.FreeTextures();
        free(renderer.m_zBuffer);
        audio.FreeEffects();
        audio.FreeTracks();
        fontManager.FreeFonts();
    }

    void Quit(SDL_Window* window, SDL_Renderer* renderer)
    {
        SDL_DestroyWindow(window);
        SDL_DestroyRenderer(renderer);
    }

    std::string GetFramesPerSecond(const float frameDelta)
    {
        return std::to_string(static_cast<int>((1.0f / frameDelta)));
    }

    float GetFrameTime(const float oldTime, const float curTime)
    {
        float frameTime = (curTime - oldTime) / 1000.0f;
        return frameTime;
    }

    std::string GetSDLAssetsFolderPath()
    {
        std::string filePath;
        const char* platform = SDL_GetPlatform();

        // strcmp returns 0 if two strings are identical
        if (strcmp(platform, "Windows") == 0)
        {
            filePath = "assets/";
        }
        else if (strcmp(platform, "Mac OS X") == 0)
        {
            filePath = "../Resources/";
        }
        else
        {
            std::cerr << "Invalid platform: " << platform << std::endl;
            filePath = "INVALID PLATFORM";
        }

        return filePath;
    }

    void ClearFile(const std::string& fileName)
    {
        std::ofstream outFile(fileName, std::ofstream::out | std::ofstream::trunc);
        outFile.close();
    }

    void WriteLineToFile(const std::string& fileName, const std::string& line)
    {
        std::ofstream outFile(fileName, std::ios::app);
        outFile << line << std::endl;
        outFile.close();
    }

    std::vector<std::string> GetFilesInDirectory(const std::string& directoryPath)
    {
        std::vector<std::string> files;
        DIR* dir;
        struct dirent* entry;

        if ((dir = opendir(directoryPath.c_str())) != nullptr)
        {
            while ((entry = readdir(dir)) != nullptr)
            {
                if (entry->d_type == DT_REG)
                {  // regular file
                    files.emplace_back(entry->d_name);
                }
            }
            closedir(dir);
        }
        else
        {
            std::cerr << "Error opening directory: " << directoryPath << std::endl;
        }

        return files;
    }

    std::vector<std::string> GetFoldersInDirectory(const std::string& directoryPath)
    {
        std::vector<std::string> folders;
        DIR* dir;
        struct dirent* entry;

        if ((dir = opendir(directoryPath.c_str())) != nullptr)
        {
            while ((entry = readdir(dir)) != nullptr)
            {
                if (entry->d_type == DT_DIR)
                {  // directory
                    std::string folderName = entry->d_name;
                    if (folderName != "." && folderName != "..")
                    {
                        folders.push_back(folderName);
                    }
                }
            }
            closedir(dir);
        }
        else
        {
            std::cerr << "Error opening directory: " << directoryPath << std::endl;
        }

        return folders;
    }
}