#pragma once

#include "Common.h"
#include "Vector.h"

namespace MiniFPS
{
    class Button
    {
    public:
        Vec2 m_pos;
        float m_width;
        float m_height;

        Button();

        Button(const Vec2& pos, float width, float height);

        bool Pressed(int mouseX, int mouseY);

        float GetLeftBound();

        float GetRightBound();

        float GetBottomBound();

        float GetTopBound();
    };
}