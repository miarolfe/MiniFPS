#pragma once

#include "Common.h"
#include "Vector.h"

namespace MiniFPS {
    class Enemy {
    public:
        Enemy();
        explicit Enemy(const FloatVector2& pos, short textureID = -1);
        void Update();

        FloatVector2 pos;
        short textureId = -1;
    };
}

