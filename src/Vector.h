#pragma once

#include "Point.h"

namespace MiniFPS {
    class Vector2 {
        Vector2(float x, float y);
        Vector2(FloatPoint point);

        float x;
        float y;

        float Length();
        void Normalize();
    };
}