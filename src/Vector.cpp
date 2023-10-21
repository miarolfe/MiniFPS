#include "Vector.h"

namespace MiniFPS
{
    Vec2::Vec2() : x(0), y(0)
    {}

    Vec2::Vec2(const float x, const float y) : x(x), y(y)
    {}

    Vec2::Vec2(const Vec2Int &intVector) : x(static_cast<float>(intVector.x)), y(static_cast<float>(intVector.y))
    {}

    Vec2::Vec2(const Vec2 &floatVector) = default;

    float Vec2::Length() const
    {
        return std::sqrt((x * x) + (y * y));
    }

    float Vec2::Distance(const Vec2 &otherVector) const
    {
        return std::sqrt((x - otherVector.x) * (x - otherVector.x) + (y - otherVector.y) * (y - otherVector.y));
    }


    float Vec2::DotProduct(const Vec2 &v1, const Vec2 &v2)
    {
        return (v1.x * v2.x) + (v1.y * v2.y);
    }

    void Vec2::Normalize()
    {
        const float length = Length();

        if (length != 0)
        {
            x = x / length;
            y = y / length;
        }
    }

    Vec2 &Vec2::operator=(const Vec2 &otherVector)
    {
        if (this != &otherVector)
        {
            x = otherVector.x;
            y = otherVector.y;
        }
        return *this;
    }

    Vec2 Vec2::operator+(const Vec2 &otherVector) const
    {
        return {x + otherVector.x, y + otherVector.y};
    }

    Vec2 &Vec2::operator+=(const Vec2 &otherVector)
    {
        x += otherVector.x;
        y += otherVector.y;
        return *this;
    }

    Vec2 Vec2::operator-(const Vec2 &otherVector) const
    {
        return {x - otherVector.x, y - otherVector.y};
    }

    Vec2 &Vec2::operator-=(const Vec2 &otherVector)
    {
        x -= otherVector.x;
        y -= otherVector.y;
        return *this;
    }

    Vec2 Vec2::operator*(const float value) const
    {
        return {x * value, y * value};
    }

    float Vec2::operator*(const Vec2 &otherVector) const
    {
        return (x * otherVector.y - y * otherVector.x);
    }

    Vec2Int::Vec2Int() : x(0), y(0)
    {}

    Vec2Int::Vec2Int(int x, int y) : x(x), y(y)
    {}

    Vec2Int::Vec2Int(const Vec2 &floatVector) : x(static_cast<int>(floatVector.x)), y(static_cast<int>(floatVector.y))
    {}

    Vec2Int::Vec2Int(const Vec2Int &vector) = default;

    Vec2Int &Vec2Int::operator=(const MiniFPS::Vec2Int &otherVector)
    {
        if (this != &otherVector)
        {
            x = otherVector.x;
            y = otherVector.y;
        }
        return *this;
    }

    bool Vec2Int::operator==(const Vec2Int &otherVector)
    {
        return (this->x == otherVector.x && this->y == otherVector.y);
    }

    Vec2Int Vec2Int::operator+(const Vec2Int &otherVector) const
    {
        return {x + otherVector.x, y + otherVector.y};
    }

    Vec2Int &Vec2Int::operator+=(const Vec2Int &otherVector)
    {
        x += otherVector.x;
        y += otherVector.y;
        return *this;
    }

    Vec2Int Vec2Int::operator-(const Vec2Int &otherVector) const
    {
        return {x - otherVector.x, y - otherVector.y};
    }

    Vec2Int &Vec2Int::operator-=(const Vec2Int &otherVector)
    {
        x -= otherVector.x;
        y -= otherVector.y;
        return *this;
    }
}