#pragma once

#include "Common.h"
#include "Vector.h"

namespace MiniFPS
{
    struct Settings
    {
        string version;
        int screenWidth;
        int screenHeight;
        int renderDistance;
        bool vSync;
        float fieldOfView;
        float speedModifier;
        float rotationModifier;
        Vec2 playerStartPos;
        float effectVolume;
        float musicVolume;
        string levelPath;
        std::vector<std::pair<string, string>> fontPaths;

        Settings();

        Settings(const string& version, int screenWidth, int screenHeight,
                 int renderDistance, bool vSync, float fieldOfView, float speedModifier,
                 float rotationModifier, const Vec2& playerStartPos,
                 float effectVolume, float musicVolume,
                 const string& levelPath, const std::map<string, string>& fontPaths);

        static Settings LoadSettings(const string& assetsFilePath, const string& settingsFilePath);
    };

    void LoadTextures();
}