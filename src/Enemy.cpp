#include "Enemy.h"

namespace MiniFPS
{
    Enemy::Enemy(const MiniFPS::Vec2& pos, short textureId) : m_pos(pos), m_textureId(textureId)
    {}

    void Enemy::Update(float frameDelta)
    {
        if (!m_waypoints.empty())
        {
            if (m_pos.Distance(m_waypoints.front()) < WAYPOINT_DISTANCE_THRESHOLD)
            {
                m_waypoints.pop();
            }
        }

        if (!m_waypoints.empty())
        {
            Vec2 movementDirection = m_waypoints.front() - m_pos;
            movementDirection.Normalize();

            m_pos += (movementDirection * frameDelta * MOVEMENT_SPEED);
        }
    }

    bool Enemy::IsVisible()
    {
        return m_visible;
    }

    void Enemy::SetVisible(bool status)
    {
        m_visible = status;
    }

    bool Enemy::SetDestination(MiniFPS::Level* level, const MiniFPS::Vec2 &goalPos)
    {
        Vec2Int pathStartPos = Vec2Int(m_pos);
        Vec2Int pathEndPos = Vec2Int(goalPos);
        Path path = CalculatePath(level, pathStartPos, pathEndPos);

        if (path.valid)
        {
            for (const Vec2& waypoint : path.waypoints)
            {
                m_waypoints.push(waypoint);
            }
        }
        else
        {
            LogHandler::LogWarning("Could not generate path");
        }

        return path.valid;
    }
}
