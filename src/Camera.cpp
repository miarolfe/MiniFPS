#include <cstddef>
#include <cmath>
#include "Camera.h"

float Camera::CalculateAspectRatio(size_t width, size_t height) {
    return static_cast<float>(width) / static_cast<float>(height);
}

float Camera::CalculateVerticalFieldOfView(float aspectRatio, float horizontalFieldOfView) {
    return(2.0f * atan(tan(horizontalFieldOfView / 2.0f) / aspectRatio));
}

Camera::Camera(float _x, float _y, float _angle, float _horizontalFieldOfView, size_t _viewportWidth, size_t _viewportHeight,
               float _rayIncrement, size_t _maxRendererDistance, float _distanceToProjectionPlane) {
    x = _x;
    y = _y;
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

