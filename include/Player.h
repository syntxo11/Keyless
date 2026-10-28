#pragma once

#include "Entity.h"

#include <chrono>
#include <memory>
#include <vector>

class Enemy;

class Player : public Entity {
public:
    enum State { left, right, up, down, jump, stay } state = stay;
    float attackCooldown = 0.0f;
    bool isAttacking = false;
    float attackFrame = 0.0f;
    bool attackHitDone = false;
    bool facingRight = true;
    int keys = 0;
    float damageTimer = 0.0f;
    float damageCooldown = 0.0f;
    float attackDamage = 50.0f;
    std::chrono::steady_clock::time_point powerUpEndTime;
    bool powerUpActive = false;
    bool showEndScreen = false;
    float idlePhase = 0.0f;
    float idleVisualOffsetY = 0.0f;

    Player(sf::Image &image, float X, float Y, int W, int H, sf::String Name);

    [[nodiscard]] sf::FloatRect getHitbox() const;
    [[nodiscard]] sf::FloatRect getAttackHitbox() const;
    bool takeDamage(int amount);
    void setWalkFrame(float time);
    void updateIdleBreathing(float time);
    void setAttackFrame();
    void control(float time);
    void checkCollisionWithMap(float Dx, float Dy);
    void handleAttack(std::vector<std::unique_ptr<Enemy>> &enemies, float time);
    void update(float time, std::vector<std::unique_ptr<Enemy>> &enemies);
};
