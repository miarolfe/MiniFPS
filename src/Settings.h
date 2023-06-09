#pragma once

#include <cstddef>
#include <fstream>
#include <vector>
#include <map>
#include <json.hpp>

using json = nlohmann::json;
using string = std::string;

namespace MiniFPS {
    struct Settings {
        std::string version;
        size_t screenWidth;
        size_t screenHeight;
        float renderRayIncrement;
        size_t renderDistance;
        bool vSync;
        float fieldOfView;
        float speedModifier;
        float rotationModifier;
        float playerStartX;
        float playerStartY;
        float playerStartAngle;
        float playerDistanceToProjectionPlane;
        string levelPath;
        std::vector<std::pair<string, string>> fontPaths;

        Settings(std::string version, size_t screenWidth, size_t screenHeight, float renderRayIncrement,
                 size_t renderDistance, bool vSync, float fieldOfView, float speedModifier,
                 float rotationModifier, float playerStartX, float playerStartY,
                 float playerStartAngle, float playerDistanceToProjectionPlane,
                 string levelPath, const std::map<string, string> &fontPaths);

        Settings();

        /**
         * Load game settings from a file.
         * @param assetsFilePath The path to the assets folder.
         * @param settingsFilePath The path to the settings file within the assets folder.
         * @return A Settings object loaded with the settings from the file.
         */
        static Settings LoadSettings(string assetsFilePath, string settingsFilePath);
    };
}