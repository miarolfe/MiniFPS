#pragma once

#include "Common.h"
#include "Vector.h"

namespace MiniFPS
{
    class Enemy
    {
    public:
        explicit Enemy(const Vec2& pos, short textureID = -1);

        void Update(float frameDelta);

        bool IsVisible();

        void SetVisible(bool status);

        Vec2 m_pos;
        short m_textureId = -1;

    private:
        bool m_visible = true;
    };
}

