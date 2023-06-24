#include <cmath>
#include "Camera.h"

namespace MiniFPS {
    float Camera::CalculateAspectRatio(size_t width, size_t height) {
        return static_cast<float>(width) / static_cast<float>(height);
    }

    float Camera::CalculateVerticalFieldOfView(float aspectRatio, float horizontalFieldOfView) {
        return(2.0f * atan(tan(horizontalFieldOfView / 2.0f) / aspectRatio));
    }

    Camera::Camera(float _x, float _y, float _angle, float _horizontalFieldOfView, int _viewportWidth, int _viewportHeight,
                   float _rayIncrement, size_t _maxRendererDistance, float _distanceToProjectionPlane) {
        pos = {_x, _y};
        angle = _angle;
        horizontalFieldOfView = _horizontalFieldOfView;
        viewportWidth = _viewportWidth;
        viewportHeight = _viewportHeight;
        rayIncrement = _rayIncrement;
        maxRenderDistance = _maxRendererDistance;
        distanceToProjectionPlane = _distanceToProjectionPlane;
        verticalFieldOfView = CalculateVerticalFieldOfView(CalculateAspectRatio(viewportWidth, viewportHeight), horizontalFieldOfView);
        aspectRatio = CalculateAspectRatio(viewportWidth, viewportHeight);
    }

    Camera::Camera() {
        pos = {0, 0};
        angle = 0;
        horizontalFieldOfView = 0;
        viewportWidth = 0;
        viewportHeight = 0;
        rayIncrement = 0;
        maxRenderDistance = 0;
        distanceToProjectionPlane = 0;
        verticalFieldOfView = 0;
        aspectRatio = 0;
    }
}


