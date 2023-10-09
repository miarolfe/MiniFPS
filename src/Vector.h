#pragma once

#include "Common.h"
#include "Point.h"

namespace MiniFPS
{
    // Forward declarations so conversion constructors can exist
    class Vec2;

    class Vec2Int;

    class Vec2
    {
    public:
        Vec2();

        Vec2(float x, float y);

        explicit Vec2(FloatPoint point);

        explicit Vec2(const Vec2Int &intVector);

        Vec2(const Vec2 &floatVector);

        float x;
        float y;

        float Length();

        float Distance(const Vec2 &otherVector) const;

        static float DotProduct(const Vec2 &v1, const Vec2 &v2);

        void Normalize();

        Vec2 &operator=(const Vec2 &otherVector);

        Vec2 operator+(Vec2 const &otherVector);

        Vec2 &operator+=(Vec2 const &otherVector);

        Vec2 operator-(Vec2 const &otherVector);

        Vec2 &operator-=(Vec2 const &otherVector);

        Vec2 operator*(float value);

        float operator*(const Vec2 &otherVector);
    };

    class Vec2Int
    {
    public:
        Vec2Int();

        Vec2Int(int x, int y);

        explicit Vec2Int(IntPoint point);

        explicit Vec2Int(const Vec2 &floatVector);

        Vec2Int(const Vec2Int &vector);

        int x;
        int y;

        Vec2Int operator+(Vec2Int const &otherVector);

        Vec2Int operator-(Vec2Int const &otherVector);
    };
}