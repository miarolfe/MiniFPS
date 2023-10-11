#pragma once

#include "AudioHandler.h"
#include "Common.h"
#include "LogHandler.h"
#include "Renderer.h"
#include "Settings.h"

namespace MiniFPS
{
    const float PI_180 = (1.0 / 180.0) * M_PI;

    bool InitWindow(SDL_Window** window, int screenWidth, int screenHeight);

    bool InitRenderer(SDL_Window* window, SDL_Renderer** renderer, bool vSync);

    bool InitSDL();

    void ShutdownSDL();

    bool InitSDLImage();

    void ShutdownSDLImage();

    bool InitSDLMixer();

    void ShutdownSDLMixer();

    bool InitSDLTTF();

    void ShutdownSDLTTF();

    void FreeResources(Renderer renderer, AudioHandler& audio, FontManager fontManager);

    void Quit(SDL_Window* window, SDL_Renderer* renderer);

    string GetFramesPerSecond(float frameDelta);

    float GetFrameTime(float oldTime, float curTime);

    string GetSDLAssetsFolderPath();

    void ClearFile(const string& fileName);

    void WriteLineToFile(const string& fileName, const string& line);

    std::vector<string> GetFilesInDirectory(const string& directoryPath);

    std::vector<string> GetFoldersInDirectory(const string& directoryPath);
}