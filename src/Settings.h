#pragma once

#include "Common.h"

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
        float playerStartX;
        float playerStartY;
        float playerStartAngle;
        float effectVolume;
        float musicVolume;
        string levelPath;
        std::vector<std::pair<string, string>> fontPaths;

        Settings();

        Settings(const string& version, int screenWidth, int screenHeight,
                 int renderDistance, bool vSync, float fieldOfView, float speedModifier,
                 float rotationModifier, float playerStartX, float playerStartY,
                 float playerStartAngle, float effectVolume, float musicVolume,
                 const string& levelPath, const std::map<string, string>& fontPaths);

        static Settings LoadSettings(const string& assetsFilePath, const string& settingsFilePath);
    };

    void LoadTextures();
}