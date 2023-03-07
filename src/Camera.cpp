#include <cstddef>
#include "Camera.h"

Camera::Camera(float _x, float _y, float _angle, float _fieldOfView, size_t _viewportWidth, size_t _viewportHeight,
               float _rayIncrement, size_t _maxRendererDistance, float _distanceToProjectionPlane) {
    x = _x;
    y = _y;
    angle = _angle;
    fieldOfView = _fieldOfView;
    viewportWidth = _viewportWidth;
    viewportHeight = _viewportHeight;
    rayIncrement = _rayIncrement;
    maxRenderDistance = _maxRendererDistance;
    distanceToProjectionPlane = _distanceToProjectionPlane;
}