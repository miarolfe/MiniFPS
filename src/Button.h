#pragma once

namespace MiniFPS {
    class Button {
    private:
        float x;
        float y;
        float width;
        float height;
    public:
        Button(float x, float y, float width, float height);

        void Draw();
        bool Pressed(float mouseX, float mouseY);
    };
}