#include "Enemy.h"

namespace MiniFPS {
    Enemy::Enemy(const MiniFPS::Vec2 &pos, short textureId) : pos(pos), textureId(textureId) {}

    void Enemy::Update() {

    }

    bool Enemy::IsVisible() {
        return visible;
    }

    void Enemy::SetVisible(bool status) {
        visible = status;
    }
}
