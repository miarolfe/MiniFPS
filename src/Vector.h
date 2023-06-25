#pragma once

#include "Point.h"

namespace MiniFPS {
    class Vector2 {
    public:
        Vector2(float x, float y);
        Vector2(FloatPoint point);

        float x;
        float y;

        float Length();
        void Normalize();
        Vector2 operator+ (Vector2 const& otherVector);
        Vector2 operator- (Vector2 const& otherVector)
    };
}