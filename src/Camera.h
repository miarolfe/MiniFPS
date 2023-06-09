#pragma once

namespace MiniFPS {
    struct Camera {
    public:
        Camera();

        Camera(float _x, float _y, float _angle, float _horizontalFieldOfView, size_t viewportWidth, size_t viewportHeight,
               float rayIncrement, size_t maxRenderDistance, float distanceToProjectionPlane);

        /**
         * Calculates the aspect ratio of a viewport.
         * @param width The width of the viewport.
         * @param height The height of the viewport.
         * @return The aspect ratio of the viewport.
         */
        static float CalculateAspectRatio(size_t width, size_t height);

        /**
         * Calculates the vertical field of view of a viewport.
         * @param aspectRatio The aspect ratio of the viewport.
         * @param horizontalFieldOfView The horizontal field of view of the viewport.
         * @return The vertical field of view of the viewport.
         */
        static float CalculateVerticalFieldOfView(float aspectRatio, float horizontalFieldOfView);

        // Position relative to level cells (where x=0, y=0 would be the top-left corner of the level
        float x, y;
        // Angle & horizontal field of view in radians
        float angle, horizontalFieldOfView;
        // Size of viewport in pixels
        size_t viewportWidth, viewportHeight;
        // Amount to increase raycast length by each iteration
        float rayIncrement;
        // Max distance for raycasts
        size_t maxRenderDistance;
        // Distance between viewport and projection plane
        float distanceToProjectionPlane;
        // Vertical field of view in radians, derived from aspect ratio and horizontal field of view
        float verticalFieldOfView;
        // Aspect ratio (width over height)
        float aspectRatio;
    };
}

