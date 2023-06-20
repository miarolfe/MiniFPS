#include <iostream>

#include "Button.h"

namespace MiniFPS {
    Button::Button() {

    }

    Button::Button(float x, float y, float width, float height) : x(x), y(y), width(width), height(height) {

    }

    void Button::Draw() {

    }

    bool Button::Pressed(float mouseX, float mouseY) {
        if (mouseX >= GetLeftBound() && mouseX <= GetRightBound() && mouseY <= GetBottomBound() && mouseY >= GetTopBound()) {
            return true;
        } else {
            return false;
        }
    }

    float Button::GetLeftBound() {
        return x - (width/2);
    }

    float Button::GetRightBound() {
        return x + (width/2);
    }

    float Button::GetBottomBound() {
        return y + (height/2);
    }

    float Button::GetTopBound() {
        return y - (height/2);
    }
}