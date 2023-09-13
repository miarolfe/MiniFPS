#include "Enemy.h"

namespace MiniFPS {
    Enemy::Enemy() = default;

    Enemy::Enemy(const MiniFPS::FloatVector2 &pos, short textureId) : pos(pos), textureId(textureId) {}

    void Enemy::Update() {

    }
}
