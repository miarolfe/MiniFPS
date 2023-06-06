#include <cstddef>
#include <fstream>
#include <sstream>
#include <json.hpp>

#include "Settings.h"

Settings::Settings(std::string version, size_t screenWidth, size_t screenHeight, float renderRayIncrement, size_t renderDistance, bool vSync,
                   float fieldOfView, float speedModifier, float rotationModifier, float playerStartX,
                   float playerStartY, float playerStartAngle, float playerDistanceToProjectionPlane,
                   std::string levelPath, const std::map<std::string, std::string> &fontPaths) {
    this->version = version;
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

    for (const auto &font: fontPaths) {
        this->fontPaths.push_back(font);
    }
}

// Use a settings path with .json extension
Settings Settings::LoadSettings(const std::string assetsFilePath, const std::string settingsFilePath) {
    std::ifstream f(assetsFilePath + settingsFilePath);

    if (!f.is_open()) {
        // TODO
    }

    json settingsAsJson = json::parse(f);
    int major, minor, patch;
    std::string version;
    size_t screenWidth, screenHeight, renderDistance;
    float renderRayIncrement, fieldOfView, speedModifier, rotationModifier, playerStartX, playerStartY,
            playerStartAngle, playerDistanceToProjectionPlane;
    std::string levelPath;
    std::map<std::string, std::string> texturePaths;
    std::map<std::string, std::string> fontPaths;
    bool vSync;

    major = settingsAsJson["game"]["version"]["major"];
    minor = settingsAsJson["game"]["version"]["minor"];
    patch = settingsAsJson["game"]["version"]["patch"];

    std::stringstream versionStringStream;
    versionStringStream << "v" << major << "." << minor << "." << patch;
    version = versionStringStream.str();

    screenWidth = settingsAsJson["graphics"]["screenWidth"];
    screenHeight = settingsAsJson["graphics"]["screenHeight"];
    renderDistance = settingsAsJson["rendering"]["renderDistance"];
    renderRayIncrement = settingsAsJson["rendering"]["renderRayIncrement"];
    fieldOfView = settingsAsJson["graphics"]["horizontalFieldOfView"];
    vSync = settingsAsJson["graphics"]["vSync"];
    speedModifier = settingsAsJson["player"]["speedModifier"];
    rotationModifier = settingsAsJson["player"]["rotationModifier"];
    playerStartX = settingsAsJson["player"]["startX"];
    playerStartY = settingsAsJson["player"]["startY"];
    playerStartAngle = settingsAsJson["player"]["startAngle"];
    playerDistanceToProjectionPlane = settingsAsJson["rendering"]["distanceToProjectionPlane"];
    levelPath = settingsAsJson["files"]["levelPath"];
    fontPaths = settingsAsJson["files"]["fontPaths"];

    Settings settings(version, screenWidth, screenHeight, renderRayIncrement, renderDistance, vSync, fieldOfView, speedModifier,
                      rotationModifier, playerStartX, playerStartY, playerStartAngle,
                      playerDistanceToProjectionPlane, levelPath, fontPaths);

    return settings;
}

Settings::Settings() {

}