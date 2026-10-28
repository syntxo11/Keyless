#pragma once

#include "Entity.h"

class Player;

class Enemy : public Entity {
public:
    enum Direction { left, right } state2 = left;
    float attackCooldown = 0.0f;
    bool isAttacking = false;
    float attackFrame = 0.0f;
    bool attackHitDone = false;
    float damageTimer = 0.0f;

    Enemy(sf::Image &image, float X, float Y, int W, int H, sf::String Name);

    [[nodiscard]] sf::FloatRect getHitbox() const;
    [[nodiscard]] sf::FloatRect getAttackHitbox() const;
    void takeDamage(int amount);
    void checkCollisionWithMap(float Dx, float Dy);
    void update(float time, Player &player);
};
