#pragma once

#include "Common.h"
#include "Point.h"

namespace MiniFPS {
    // Forward declarations so conversion constructors can exist
    class FloatVector2;
    class DoubleVector2;
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
        FloatVector2& operator+= (FloatVector2 const& otherVector);
        FloatVector2 operator- (FloatVector2 const& otherVector);\
        FloatVector2& operator-= (FloatVector2 const& otherVector);
        FloatVector2 operator* (const float value);
    };

    class DoubleVector2 {
    public:
        DoubleVector2();
        DoubleVector2(double x, double y);
        explicit DoubleVector2(FloatPoint point);
        explicit DoubleVector2(const IntVector2 &intVector);
        DoubleVector2(const DoubleVector2& floatVector);

        double x;
        double y;

        double Length();
        void Normalize();
        DoubleVector2 operator+ (DoubleVector2 const& otherVector);
        DoubleVector2 operator- (DoubleVector2 const& otherVector);
        DoubleVector2 operator* (const double value);
    };

    class IntVector2 {
    public:
        IntVector2();
        IntVector2(int x, int y);
        explicit IntVector2(IntPoint point);
        explicit IntVector2(const FloatVector2& floatVector);
        explicit IntVector2(const DoubleVector2& doubleVector);
        IntVector2(const IntVector2& vector);

        int x;
        int y;

        IntVector2 operator+ (IntVector2 const& otherVector);
        IntVector2 operator- (IntVector2 const& otherVector);
    };
}