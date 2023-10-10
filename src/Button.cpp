#include "Button.h"

namespace MiniFPS
{
    Button::Button()
    {}

    Button::Button(const Vec2& pos, float width, float height) : m_pos(pos), m_width(width), m_height(height)
    {}

    bool Button::Pressed(int mouseX, int mouseY)
    {
        if (static_cast<float>(mouseX) >= GetLeftBound() && static_cast<float>(mouseX) <= GetRightBound() &&
            static_cast<float>(mouseY) <= GetBottomBound() && static_cast<float>(mouseY) >= GetTopBound())
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    float Button::GetLeftBound()
    {
        return m_pos.x - (m_width / 2);
    }

    float Button::GetRightBound()
    {
        return m_pos.x + (m_width / 2);
    }

    float Button::GetBottomBound()
    {
        return m_pos.y + (m_height / 2);
    }

    float Button::GetTopBound()
    {
        return m_pos.y - (m_height / 2);
    }
}