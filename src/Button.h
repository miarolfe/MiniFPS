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

        void Draw();
        bool Pressed(float mouseX, float mouseY);
        float GetLeftBound();
        float GetRightBound();
        float GetBottomBound();
        float GetTopBound();
    };
}