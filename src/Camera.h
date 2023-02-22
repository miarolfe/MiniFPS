#pragma once
#ifndef MINI_FPS_CAMERA_H
#define MINI_FPS_CAMERA_H

class Camera {
public:
    Camera(float _x, float _y, float _angle, float _fieldOfView);
    // Position relative to level cells (where x=0, y=0 would be the top-left corner of the level
    float x, y;
    // Angle & field of view in radians
    float angle, fieldOfView;
};

#endif //MINI_FPS_CAMERA_H
