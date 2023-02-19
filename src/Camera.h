//
// Created by Peter Rolfe on 19/02/2023.
//

#ifndef MINI_FPS_CAMERA_H
#define MINI_FPS_CAMERA_H

class Camera {
public:
    Camera(float _x, float _y, float _angle, float _fieldOfView);
    float x, y, angle, fieldOfView;
};

#endif //MINI_FPS_CAMERA_H
