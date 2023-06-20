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
        const float leftBound = x - (width/2);
        const float rightBound = x + (width/2);
        const float bottomBound = y + (height/2);
        const float topBound = y - (height/2);

        if (mouseX >= leftBound && mouseX <= rightBound && mouseY <= bottomBound && mouseY >= topBound) {
            return true;
        } else {
            return false;
        }
    }
}