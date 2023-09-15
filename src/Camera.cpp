#include "Camera.h"

namespace MiniFPS {
    float Camera::CalculateAspectRatio(size_t width, size_t height) {
        return static_cast<float>(width) / static_cast<float>(height);
    }

    float Camera::CalculateVerticalFieldOfView(float aspectRatio, float horizontalFieldOfView) {
        return(2.0f * atan(tan(horizontalFieldOfView / 2.0f) / aspectRatio));
    }

    Camera::Camera(float _x, float _y, float _angle, float _horizontalFieldOfView, int _viewportWidth, int _viewportHeight,
                   size_t _maxRendererDistance, float _distanceToProjectionPlane) {
        pos = {_x, _y};

        // direction = {cosf(_angle), sinf(_angle)}
        direction = {-1.0f, 0.0f};
        direction.Normalize();
        plane = {0.0f,  0.66f};
        plane.Normalize();
        horizontalFieldOfView = _horizontalFieldOfView;
        viewportWidth = _viewportWidth;
        viewportHeight = _viewportHeight;
        maxRenderDistance = _maxRendererDistance;
        distanceToProjectionPlane = _distanceToProjectionPlane;
        verticalFieldOfView = CalculateVerticalFieldOfView(CalculateAspectRatio(viewportWidth, viewportHeight), horizontalFieldOfView);
        aspectRatio = CalculateAspectRatio(viewportWidth, viewportHeight);
    }

    Camera::Camera() {
        pos = {0, 0};
        direction = {0.0f, 0.0f};
        plane = {0.0f, 0.0f};
        horizontalFieldOfView = 0;
        viewportWidth = 0;
        viewportHeight = 0;
        maxRenderDistance = 0;
        distanceToProjectionPlane = 0;
        verticalFieldOfView = 0;
        aspectRatio = 0;
    }
}


