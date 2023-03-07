#ifndef MINI_FPS_SETTINGS_H
#define MINI_FPS_SETTINGS_H

#include <cstddef>
#include <fstream>
#include <json.hpp>

using json = nlohmann::json;

struct Settings {
    size_t screenWidth;
    size_t screenHeight;
    float renderRayIncrement;
    size_t renderDistance;
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
             size_t renderDistance, float fieldOfView, float speedModifier,
             float rotationModifier, float playerStartX, float playerStartY,
             float playerStartAngle, float playerDistanceToProjectionPlane,
             std::string levelPath, const std::vector<std::string> &texturePaths);
};

Settings::Settings(size_t screenWidth, size_t screenHeight, float renderRayIncrement, size_t renderDistance,
                   float fieldOfView, float speedModifier, float rotationModifier, float playerStartX,
                   float playerStartY, float playerStartAngle, float playerDistanceToProjectionPlane,
                   std::string levelPath, const std::vector<std::string> &texturePaths) {
    this->screenWidth = screenWidth;
    this->screenHeight = screenHeight;
    this->renderRayIncrement = renderRayIncrement;
    this->renderDistance = renderDistance;
    this->fieldOfView = fieldOfView;
    this->speedModifier = speedModifier;
    this->rotationModifier = rotationModifier;
    this->playerStartX = playerStartX;
    this->playerStartY = playerStartY;
    this->playerStartAngle = playerStartAngle;
    this->playerDistanceToProjectionPlane = playerDistanceToProjectionPlane;
    this->levelPath = levelPath;
    for (const std::string &texturePath: texturePaths) {
        this->texturePaths.push_back(texturePath);
    }
}

// Use a settings path with .json extension
Settings loadSettings(const std::string &assetsFilePath, const std::string &settingsFilePath) {
    std::ifstream f(assetsFilePath + settingsFilePath);
    json settingsAsJson = json::parse(f);
    size_t screenWidth, screenHeight, renderDistance;
    float renderRayIncrement, fieldOfView, speedModifier, rotationModifier, playerStartX, playerStartY,
            playerStartAngle, playerDistanceToProjectionPlane;
    std::string levelPath;
    std::vector<std::string> texturePaths;

    screenWidth = settingsAsJson["screenWidth"];
    screenHeight = settingsAsJson["screenHeight"];
    renderDistance = settingsAsJson["renderDistance"];
    renderRayIncrement = settingsAsJson["renderRayIncrement"];
    fieldOfView = settingsAsJson["fieldOfView"];
    speedModifier = settingsAsJson["speedModifier"];
    rotationModifier = settingsAsJson["rotationModifier"];
    playerStartX = settingsAsJson["playerStartX"];
    playerStartY = settingsAsJson["playerStartY"];
    playerStartAngle = settingsAsJson["playerStartAngle"];
    playerDistanceToProjectionPlane = settingsAsJson["playerDistanceToProjectionPlane"];
    levelPath = settingsAsJson["levelPath"];
    texturePaths = settingsAsJson["texturePaths"];

    Settings settings(screenWidth, screenHeight, renderRayIncrement, renderDistance, fieldOfView, speedModifier,
                      rotationModifier, playerStartX, playerStartY, playerStartAngle,
                      playerDistanceToProjectionPlane, levelPath, texturePaths);

    return settings;
}

#endif //MINI_FPS_SETTINGS_H
