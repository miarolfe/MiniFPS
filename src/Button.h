#pragma once

#include "Common.h"
#include "Vector.h"

namespace MiniFPS
{
    class Button
    {
    private:
        Vec2 m_pos;
        Vec2 m_size;

    public:
        Button();

        Button(const Vec2& pos, const Vec2& size);

        bool Pressed(const Vec2Int& mousePos) const;

        float GetLeftBound() const;

        float GetRightBound() const;

        float GetBottomBound() const;

        float GetTopBound() const;

        const Vec2& GetPos() const;

        const Vec2& GetSize() const;
    };
}