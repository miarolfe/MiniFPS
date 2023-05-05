#pragma once

#include <cstddef>
#include <fstream>
#include <json.hpp>

using json = nlohmann::json;

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
    std::string levelPath;
    std::vector<std::string> texturePaths;

    Settings(size_t screenWidth, size_t screenHeight, float renderRayIncrement,
             size_t renderDistance, bool vSync, float fieldOfView, float speedModifier,
             float rotationModifier, float playerStartX, float playerStartY,
             float playerStartAngle, float playerDistanceToProjectionPlane,
             std::string levelPath, const std::vector<std::string> &texturePaths);

    static Settings LoadSettings(std::string assetsFilePath, std::string settingsFilePath);
};