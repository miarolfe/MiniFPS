#include "Camera.h"

namespace MiniFPS
{
    Camera::Camera(const Vec2& playerStartPos, float horizontalFieldOfView, int maxRenderDistance)
    {
        this->pos = playerStartPos;
        direction = CAMERA_START_DIRECTION;
        plane = CAMERA_START_PLANE;
        direction.Normalize();
        plane.Normalize();
        this->horizontalFieldOfView = horizontalFieldOfView;
        this->width = CAMERA_RESOLUTION.x;
        this->height = CAMERA_RESOLUTION.y;
        this->maxRenderDistance = maxRenderDistance;
    }
}


