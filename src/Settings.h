#pragma once

#include <cstddef>
#include <fstream>
#include <vector>
#include <map>
#include <json.hpp>

using json = nlohmann::json;
using string = std::string;

struct Settings {
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
    std::vector<string> texturePaths;
    std::vector<std::pair<string, string>> fontPaths;

    Settings(size_t screenWidth, size_t screenHeight, float renderRayIncrement,
             size_t renderDistance, bool vSync, float fieldOfView, float speedModifier,
             float rotationModifier, float playerStartX, float playerStartY,
             float playerStartAngle, float playerDistanceToProjectionPlane,
             string levelPath, const std::vector<string> &texturePaths,
             const std::map<std::string, std::string> &fontPaths);

    Settings();

    static Settings LoadSettings(string assetsFilePath, string settingsFilePath);
};