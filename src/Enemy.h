#pragma once

#include "Common.h"
#include "Vector.h"

namespace MiniFPS {
    class Enemy {
    public:
        Enemy(const DoubleVector2& pos);
        void Update();

    private:
        DoubleVector2 pos;
    };
}

