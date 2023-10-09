#pragma once

#include "Common.h"
#include "Point.h"

namespace MiniFPS
{
    class Button
    {
    public:
        FloatPoint pos;
        float width;
        float height;

        Button();

        Button(FloatPoint pos, float width, float height);

        bool Pressed(int mouseX, int mouseY);

        float GetLeftBound();

        float GetRightBound();

        float GetBottomBound();

        float GetTopBound();
    };
}