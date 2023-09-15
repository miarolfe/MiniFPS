#pragma once

#include "Common.h"

namespace MiniFPS {
    struct Settings {
        std::string version;
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
        float playerDistanceToProjectionPlane;
        float effectVolume;
        float musicVolume;
        std::string levelPath;
        std::vector<std::pair<std::string, std::string>> fontPaths;

        Settings();
        Settings(const std::string& version, int screenWidth, int screenHeight,
                 int renderDistance, bool vSync, float fieldOfView, float speedModifier,
                 float rotationModifier, float playerStartX, float playerStartY,
                 float playerStartAngle, float playerDistanceToProjectionPlane, float effectVolume, float musicVolume,
                 const std::string& levelPath, const std::map<std::string, std::string> &fontPaths);

        /**
         * Load game settings from a file.
         * @param assetsFilePath The path to the assets folder.
         * @param settingsFilePath The path to the settings file within the assets folder.
         * @return A Settings object loaded with the settings from the file.
         */
        static Settings LoadSettings(const std::string& assetsFilePath, const std::string& settingsFilePath);
    };

    void LoadTextures();
}