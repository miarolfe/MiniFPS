#pragma once
#ifndef MINI_FPS_CAMERA_H
#define MINI_FPS_CAMERA_H

class Camera {
public:
    Camera(float _x, float _y, float _angle, float _fieldOfView, size_t viewportWidth, size_t viewportHeight,
           float rayIncrement, size_t maxRenderDistance, float distanceToProjectionPlane);

    // Position relative to level cells (where x=0, y=0 would be the top-left corner of the level
    float x, y;
    // Angle & field of view in radians
    float angle, fieldOfView;
    // Size of viewport in pixels
    size_t viewportWidth, viewportHeight;
    // Amount to increase raycast length by each iteration
    float rayIncrement;
    // Max distance for raycasts
    size_t maxRenderDistance;
    // Distance between viewport and projection plane
    float distanceToProjectionPlane;
};

#endif //MINI_FPS_CAMERA_H
