#pragma once

#include "AudioHandler.h"
#include "Common.h"
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

    void FreeResources(Renderer renderer, AudioHandler audio, FontManager fontManager);

    void Quit(SDL_Window* window, SDL_Renderer* renderer);

    std::string GetFramesPerSecond(float frameDelta);

    float GetFrameTime(float oldTime, float curTime);

    std::string GetSDLAssetsFolderPath();

    void ClearFile(const std::string& fileName);

    void WriteLineToFile(const std::string& fileName, const std::string& line);

    std::vector<std::string> GetFilesInDirectory(const std::string& directoryPath);

    std::vector<std::string> GetFoldersInDirectory(const std::string& directoryPath);

    // bool IsPointInRightAngledTriangle(FloatPoint point, FloatPoint vertex1, FloatPoint vertex2, FloatPoint vertex3);
}