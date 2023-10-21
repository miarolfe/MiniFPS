#include "Button.h"

namespace MiniFPS
{
    Button::Button() = default;

    Button::Button(const Vec2& pos, const Vec2& size) : m_pos(pos), m_size(size)
    {}

    bool Button::Pressed(const Vec2Int& mousePos) const
    {
        if (static_cast<float>(mousePos.x) >= GetLeftBound() && static_cast<float>(mousePos.x) <= GetRightBound() &&
            static_cast<float>(mousePos.y) <= GetBottomBound() && static_cast<float>(mousePos.y) >= GetTopBound())
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    float Button::GetLeftBound() const
    {
        return m_pos.x - (m_size.x / 2);
    }

    float Button::GetRightBound() const
    {
        return m_pos.x + (m_size.x / 2);
    }

    float Button::GetBottomBound() const
    {
        return m_pos.y + (m_size.y / 2);
    }

    float Button::GetTopBound() const
    {
        return m_pos.y - (m_size.y / 2);
    }

    const Vec2& Button::GetPos() const
    {
        return m_pos;
    }

    const Vec2& Button::GetSize() const
    {
        return m_size;
    }
}