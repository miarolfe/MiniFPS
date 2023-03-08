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
};

Settings::Settings(size_t screenWidth, size_t screenHeight, float renderRayIncrement, size_t renderDistance, bool vSync,
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
    this->vSync = vSync;
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
    bool vSync;

    screenWidth = settingsAsJson["graphics"]["screenWidth"];
    screenHeight = settingsAsJson["graphics"]["screenHeight"];
    renderDistance = settingsAsJson["rendering"]["renderDistance"];
    renderRayIncrement = settingsAsJson["rendering"]["renderRayIncrement"];
    fieldOfView = settingsAsJson["graphics"]["fieldOfView"];
    speedModifier = settingsAsJson["player"]["speedModifier"];
    rotationModifier = settingsAsJson["player"]["rotationModifier"];
    playerStartX = settingsAsJson["player"]["startX"];
    playerStartY = settingsAsJson["player"]["startY"];
    playerStartAngle = settingsAsJson["player"]["startAngle"];
    playerDistanceToProjectionPlane = settingsAsJson["rendering"]["distanceToProjectionPlane"];
    levelPath = settingsAsJson["files"]["levelPath"];
    texturePaths = settingsAsJson["files"]["texturePaths"];
    vSync = settingsAsJson["graphics"]["vSync"];

    Settings settings(screenWidth, screenHeight, renderRayIncrement, renderDistance, vSync, fieldOfView, speedModifier,
                      rotationModifier, playerStartX, playerStartY, playerStartAngle,
                      playerDistanceToProjectionPlane, levelPath, texturePaths);

    return settings;
}

#endif //MINI_FPS_SETTINGS_H
