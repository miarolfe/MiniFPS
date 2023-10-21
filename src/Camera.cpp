#include "Camera.h"

namespace MiniFPS
{
    Camera::Camera(const Vec2& playerStartPos, float horizontalFieldOfView, int maxRenderDistance, const Vec2Int& cameraResolution)
    {
        this->pos = playerStartPos;
        direction = CAMERA_START_DIRECTION;
        plane = CAMERA_START_PLANE;
        direction.Normalize();
        plane.Normalize();
        this->horizontalFieldOfView = horizontalFieldOfView;
        this->width = cameraResolution.x;
        this->height = cameraResolution.y;
        this->maxRenderDistance = maxRenderDistance;
    }
}


