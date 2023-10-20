#pragma once

#include "Common.h"
#include "Vector.h"

namespace MiniFPS
{
    const Vec2Int CAMERA_RESOLUTION {800, 445};
    const Vec2 CAMERA_START_DIRECTION {-1.0f, 0.0f};
    const Vec2 CAMERA_START_PLANE {0.0f, 0.66f};

    struct Camera
    {
    public:
        Camera() {}
        Camera(const Vec2& playerStartPos, float horizontalFieldOfView, int maxRenderDistance);

        // Position relative to level cells (where x=0, y=0 would be the top-left corner of the level)
        Vec2 pos;

        // Horizontal field of view in radians
        float horizontalFieldOfView;

        Vec2 direction;
        Vec2 plane;

        // Size of viewport in pixels
        int width, height;
        // Max distance for raycasts
        int maxRenderDistance;
    };
}

