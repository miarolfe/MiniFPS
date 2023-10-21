#pragma once

#include "Common.h"
#include "Pathfinding.h"
#include "Vector.h"

namespace MiniFPS
{
    const float WAYPOINT_DISTANCE_THRESHOLD = 0.05f;
    const float MOVEMENT_SPEED = 2.0f;

    class Enemy
    {
    public:
        explicit Enemy(const Vec2& pos, short textureID = -1);

        void Update(float frameDelta);

        bool IsVisible();

        void SetVisible(bool status);

        bool SetDestination(Level* level, const Vec2& goalPos);

        Vec2 m_pos;
        short m_textureId = -1;

    private:
        bool m_visible = true;
        std::queue<Vec2> m_waypoints;
    };
}

