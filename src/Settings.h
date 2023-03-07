#ifndef MINI_FPS_SETTINGS_H
#define MINI_FPS_SETTINGS_H

#include <cstddef>

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
    const char* levelPath;
    Settings(size_t screenWidth, size_t screenHeight, float renderRayIncrement,
                       size_t renderDistance, float fieldOfView, float speedModifier,
                       float rotationModifier, float playerStartX, float playerStartY,
                       float playerStartAngle, float playerDistanceToProjectionPlane,
                       const char* levelPath);
};

Settings::Settings(size_t screenWidth, size_t screenHeight, float renderRayIncrement, size_t renderDistance,
                   float fieldOfView, float speedModifier, float rotationModifier, float playerStartX,
                   float playerStartY, float playerStartAngle, float playerDistanceToProjectionPlane,
                   const char *levelPath) {
    this->screenWidth = screenHeight;
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
}

#endif //MINI_FPS_SETTINGS_H
