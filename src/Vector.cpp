#include <cmath>

#include "Vector.h"

namespace MiniFPS {
    Vector2::Vector2(const float x, const float y) : x(x), y(y) {}

    Vector2::Vector2(FloatPoint point) : x(point.x), y(point.y) {}

    float Vector2::Length() {
        return sqrt((x*x) + (y*y));
    }

    void Vector2::Normalize() {
        const float length = Length();
        x = x / length;
        y = y / length;
    }
}