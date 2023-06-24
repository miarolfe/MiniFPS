#include <iostream>

#include "Button.h"
#include "Point.h"

namespace MiniFPS {
    Button::Button() {}
    
    Button::Button(FloatPoint pos, float width, float height) : pos(pos), width(width), height(height) {}

    bool Button::Pressed(int mouseX, int mouseY) {
        if (static_cast<float>(mouseX) >= GetLeftBound() && static_cast<float>(mouseX) <= GetRightBound() &&
            static_cast<float>(mouseY) <= GetBottomBound() && static_cast<float>(mouseY) >= GetTopBound()) {
            return true;
        } else {
            return false;
        }
    }

    float Button::GetLeftBound() {
        return pos.x - (width/2);
    }

    float Button::GetRightBound() {
        return pos.x + (width/2);
    }

    float Button::GetBottomBound() {
        return pos.y + (height/2);
    }

    float Button::GetTopBound() {
        return pos.y - (height/2);
    }
}