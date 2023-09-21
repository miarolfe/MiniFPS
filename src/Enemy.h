#pragma once

#include "Common.h"
#include "Vector.h"

namespace MiniFPS {
    class Enemy {
    public:
        explicit Enemy(const Vec2& pos, short textureID = -1);
        void Update();
        bool IsVisible();
        void SetVisible(bool status);

        Vec2 pos;
        short textureId = -1;

    private:
        bool visible = true;
    };
}

