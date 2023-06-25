#pragma once

#include "Point.h"

namespace MiniFPS {
    // Forward declarations so conversion constructors can exist
    class FloatVector2;
    class IntVector2;

    class FloatVector2 {
    public:
        FloatVector2();
        FloatVector2(float x, float y);
        explicit FloatVector2(FloatPoint point);
        explicit FloatVector2(const IntVector2 &intVector);
        FloatVector2(const FloatVector2& floatVector);

        float x;
        float y;

        float Length();
        void Normalize();
        FloatVector2 operator+ (FloatVector2 const& otherVector);
        FloatVector2 operator- (FloatVector2 const& otherVector);
        FloatVector2 operator* (const float value);
    };

    class IntVector2 {
    public:
        IntVector2();
        IntVector2(int x, int y);
        explicit IntVector2(IntPoint point);
        explicit IntVector2(const FloatVector2 &floatVector);
        IntVector2(const IntVector2& vector);

        int x;
        int y;

        IntVector2 operator+ (IntVector2 const& otherVector);
        IntVector2 operator- (IntVector2 const& otherVector);
    };
}