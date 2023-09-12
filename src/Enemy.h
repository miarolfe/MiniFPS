#pragma once

#include "Common.h"
#include "Vector.h"

namespace MiniFPS {
    class Enemy {
    public:
        explicit Enemy(const DoubleVector2& pos, short textureID = -1);
        void Update();

        DoubleVector2 pos;
        short textureId = -1;
    };
}

