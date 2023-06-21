#pragma once

namespace MiniFPS {
    class Button {
    public:
        float x;
        float y;
        float width;
        float height;

        Button();
        Button(float x, float y, float width, float height);

        bool Pressed(int mouseX, int mouseY);
        float GetLeftBound();
        float GetRightBound();
        float GetBottomBound();
        float GetTopBound();
    };
}