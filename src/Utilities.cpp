#include "Utilities.h"
#include "Audio.h"

namespace MiniFPS
{
    bool InitializeSDLSubsystems()
    {
        bool successfulInitialization = true;

        if (!InitializeSDL())
        {
            std::cerr << "SDL could not be initialized:" << SDL_GetError();
            successfulInitialization = false;
        }

        if (!InitializeSDLImage())
        {
            std::cerr << "SDL_image could not be initialized" << std::endl;
            successfulInitialization = false;
        }

        if (!InitializeSDLMixer())
        {
            std::cerr << "SDL_mixer could not be initialized" << std::endl;
            successfulInitialization = false;
        }

        if (!InitializeSDLTTF())
        {
            std::cerr << "SDL_ttf could not be initialized" << std::endl;
            successfulInitialization = false;
        }

        return successfulInitialization;
    }

    void DeactivateSDLSubsystems()
    {
        SDL_Quit();
        IMG_Quit();

        Mix_CloseAudio();
        Mix_Quit();

        // TODO: Free fonts
        TTF_Quit();
    }

    void FreeResources(Renderer renderer, Audio audio, FontManager fontManager)
    {
        renderer.FreeTextures();
        // TODO: Free zBuffer
        audio.FreeEffects();
        audio.FreeTracks();
        fontManager.FreeFonts();
    }

    bool InitializeSDL()
    {
        bool successfulInitialization = true;

        if (SDL_Init((SDL_INIT_VIDEO | SDL_INIT_AUDIO)) < 0)
        {
            successfulInitialization = false;
        }

        return successfulInitialization;
    }

    bool InitializeWindowAndRenderer(SDL_Window** window, SDL_Renderer** renderer, int screenWidth,
                                     int screenHeight, bool vSync)
    {
        bool successfulInitialization = true;

        *window = SDL_CreateWindow("MiniFPS",
                                   SDL_WINDOWPOS_CENTERED,
                                   SDL_WINDOWPOS_CENTERED,
                                   screenWidth,
                                   screenHeight,
                                   SDL_WINDOW_SHOWN);

        if (*window == nullptr)
        {
            successfulInitialization = false;
        }

        if (vSync)
        {
            *renderer = SDL_CreateRenderer(*window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
        }
        else
        {
            *renderer = SDL_CreateRenderer(*window, -1, SDL_RENDERER_ACCELERATED);
        }

        if (*renderer == nullptr)
        {
            successfulInitialization = false;
        }

        // Capture mouse cursor and enable relative mouse mode
        SDL_SetWindowGrab(*window, SDL_TRUE);

        return successfulInitialization;
    }

    bool InitializeSDLImage()
    {
        bool successful_initialization = true;

        if (!IMG_Init(IMG_INIT_PNG))
        {
            successful_initialization = false;
        }

        return successful_initialization;
    }

    bool InitializeSDLMixer()
    {
        bool successfulInitialization = true;

        if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) != 0)
        {
            successfulInitialization = false;
        }

        return successfulInitialization;
    }

    bool InitializeSDLTTF()
    {
        bool successfulInitialization = true;

        if (TTF_Init() != 0)
        {
            successfulInitialization = false;
        }

        return successfulInitialization;
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

    void ClearFile(const std::string &fileName)
    {
        std::ofstream outFile(fileName, std::ofstream::out | std::ofstream::trunc);
        outFile.close();
    }

    void WriteLineToFile(const std::string &fileName, const std::string &line)
    {
        std::ofstream outFile(fileName, std::ios::app);
        outFile << line << std::endl;
        outFile.close();
    }

    std::vector<std::string> GetFilesInDirectory(const std::string &directoryPath)
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

    std::vector<std::string> GetFoldersInDirectory(const std::string &directoryPath)
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

    float CalculateCrossProduct(const FloatPoint v1, const FloatPoint v2)
    {
        return ((v1.x * v2.y) - (v2.x * v1.y));
    }

    bool IsPointInRightAngledTriangle(const FloatPoint point, const FloatPoint vertex1, const FloatPoint vertex2,
                                      const FloatPoint vertex3)
    {
        const FloatPoint v1 = {point.x - vertex1.x, point.y - vertex1.y};
        const FloatPoint v2 = {point.x - vertex2.x, point.y - vertex2.y};
        const FloatPoint v3 = {point.x - vertex3.x, point.y - vertex3.y};

        // Calculate the cross products of these vectors
        const float cp1 = CalculateCrossProduct(v1, v2);
        const float cp2 = CalculateCrossProduct(v2, v3);
        const float cp3 = CalculateCrossProduct(v3, v1);

        // If all cross products have the same sign, the point is inside the triangle
        return ((cp1 >= 0 && cp2 >= 0 && cp3 >= 0) || (cp1 <= 0 && cp2 <= 0 && cp3 <= 0));
    }
}