#include "Enemy.h"

namespace MiniFPS
{
    Enemy::Enemy(const MiniFPS::Vec2& pos, short textureId) : m_pos(pos), m_textureId(textureId)
    {}

    void Enemy::Update(float frameDelta)
    {
        // m_pos.x += frameDelta;
    }

    bool Enemy::IsVisible()
    {
        return m_visible;
    }

    void Enemy::SetVisible(bool status)
    {
        m_visible = status;
    }
}
