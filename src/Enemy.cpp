#include "Enemy.h"

#include "Player.h"
#include "game_config.h"
#include "gameplay_utils.h"
#include "map.h"

#include <algorithm>
#include <cmath>

using namespace sf;

Enemy::Enemy(Image &image, float X, float Y, int W, int H, String Name)
    : Entity(image, X, Y, W, H, std::move(Name)) {
    if (name == "EasyEnemy") {
        sprite.setTextureRect(IntRect(0, 50, w, h));
    }
}

FloatRect Enemy::getHitbox() const {
    return {x + 8.0f, y + 5.0f,
            std::max(1.0f, static_cast<float>(w) - 16.0f),
            std::max(1.0f, static_cast<float>(h) - 8.0f)};
}

FloatRect Enemy::getAttackHitbox() const {
    const float hitY = y + 6.0f;
    const float hitH = std::max(1.0f, static_cast<float>(h) - 12.0f);

    if (state2 == right) {
        return {x + w - 5.0f, hitY, 30.0f, hitH};
    }
    return {x - 25.0f, hitY, 30.0f, hitH};
}

void Enemy::takeDamage(int amount) {
    if (!life) return;

    health -= amount;
    sprite.setColor(Color::Red);
    damageTimer = keyless::config::ENEMY_DAMAGE_FLASH_MS;

    if (health <= 0) {
        health = 0;
        life = false;
    }
}

void Enemy::checkCollisionWithMap(float Dx, float Dy) {
    for (int i = static_cast<int>(std::floor(y / 16.0f));
         i < static_cast<int>(std::ceil((y + h) / 16.0f)); ++i) {
        for (int j = static_cast<int>(std::floor(x / 16.0f));
             j < static_cast<int>(std::ceil((x + w) / 16.0f)); ++j) {
            if (i < 0 || i >= HEIGHT_MAP || j < 0 || j >= WIDTH_MAP) continue;

            const char tile = static_cast<char>(TileMap[i][j]);
            const bool solid = tile == 'a' || tile == 'b' || tile == 'c' || tile == 'd'
                || tile == '{' || tile == '}' || tile == '(' || tile == ')'
                || tile == '[' || tile == ']';

            if (!solid) continue;

            if (Dy > 0.0f) { y = i * 16.0f - h; dy = 0.0f; onGround = true; }
            if (Dy < 0.0f) { y = i * 16.0f + 16.0f; dy = 0.0f; }
            if (Dx > 0.0f) { x = j * 16.0f - w; }
            if (Dx < 0.0f) { x = j * 16.0f + 16.0f; }
        }
    }
}

void Enemy::update(float time, Player &player) {
    if (!life) return;

    FloatRect playerBox = player.getHitbox();
    FloatRect myBox = getHitbox();

    // A slime must never occupy the player's body. If overlap happens because
    // of an old state or a large frame delta, separate it immediately.
    FloatRect overlap;
    if (myBox.intersects(playerBox, overlap)) {
        const float myCenterX = myBox.left + myBox.width / 2.0f;
        const float playerCenterX = playerBox.left + playerBox.width / 2.0f;
        constexpr float separationPadding = 1.0f;

        const float correction = myCenterX <= playerCenterX
            ? -(overlap.width + separationPadding)
            : overlap.width + separationPadding;

        x += correction;
        checkCollisionWithMap(correction, 0.0f);
        dx = 0.0f;
        myBox = getHitbox();
    }

    const float playerCenterX = playerBox.left + playerBox.width / 2.0f;
    const float playerCenterY = playerBox.top + playerBox.height / 2.0f;
    const float myCenterX = myBox.left + myBox.width / 2.0f;
    const float myCenterY = myBox.top + myBox.height / 2.0f;
    const float distance = std::sqrt((playerCenterX - myCenterX) * (playerCenterX - myCenterX)
                                   + (playerCenterY - myCenterY) * (playerCenterY - myCenterY));

    if (distance < keyless::gameplay::ENEMY_AGGRO_DISTANCE) {
        state2 = playerCenterX >= myCenterX ? right : left;
    }

    const bool verticalOverlap = keyless::gameplay::rangesOverlap(
        myBox.top, myBox.height, playerBox.top, playerBox.height);
    const float gap = keyless::gameplay::horizontalGap(
        myBox.left, myBox.width, playerBox.left, playerBox.width);
    const bool inMeleeRange = verticalOverlap && gap <= keyless::gameplay::ENEMY_MELEE_GAP;

    if (!isAttacking) {
        if (inMeleeRange) {
            // Stay at melee distance during cooldown instead of walking toward
            // the player's centre after the first attack.
            dx = 0.0f;

            if (attackCooldown <= 0.0f) {
                isAttacking = true;
                attackFrame = 0.0f;
                attackHitDone = false;
                attackCooldown = keyless::config::ENEMY_ATTACK_COOLDOWN_MS;
            }
        } else if (distance < keyless::gameplay::ENEMY_AGGRO_DISTANCE) {
            dx = playerCenterX > myCenterX
                ? keyless::config::ENEMY_MOVE_SPEED
                : -keyless::config::ENEMY_MOVE_SPEED;
        } else {
            if (moveTimer > 0.0f) {
                moveTimer -= time;
            } else {
                if (dx == 0.0f) {
                    dx = state2 == right
                        ? keyless::config::ENEMY_MOVE_SPEED
                        : -keyless::config::ENEMY_MOVE_SPEED;
                } else {
                    dx = -dx;
                }
                state2 = dx >= 0.0f ? right : left;
                moveTimer = 3000.0f;
            }
        }
    } else {
        dx = 0.0f;
    }

    x += dx * time;
    checkCollisionWithMap(dx, 0.0f);
    y += dy * time;
    checkCollisionWithMap(0.0f, dy);
    sprite.setPosition(x + w / 2.0f, y + h / 2.0f);

    if (isAttacking) {
        attackFrame += 0.005f * time;
        const int frame = std::clamp(static_cast<int>(attackFrame), 0, 4);

        if (state2 == right) {
            sprite.setTextureRect(IntRect(89 * frame, 181, 87, 32));
        } else {
            sprite.setTextureRect(IntRect(89 * frame, 136, 87, 32));
        }

        if (!attackHitDone && attackFrame >= 3.0f) {
            if (getAttackHitbox().intersects(player.getHitbox())) {
                player.takeDamage(keyless::config::ENEMY_ATTACK_DAMAGE);
            }
            attackHitDone = true;
        }

        if (attackFrame >= 5.0f) {
            isAttacking = false;
            attackFrame = 0.0f;
            attackHitDone = false;
        }
    } else {
        currentFrame += 0.005f * time;
        while (currentFrame >= 4.0f) currentFrame -= 4.0f;

        if (state2 == right) {
            sprite.setTextureRect(IntRect(57 * static_cast<int>(currentFrame), 91, 57, 32));
        } else {
            sprite.setTextureRect(IntRect(57 * static_cast<int>(currentFrame), 50, 57, 32));
        }
    }

    if (damageTimer > 0.0f) {
        damageTimer -= time;
        if (damageTimer <= 0.0f) sprite.setColor(Color::White);
    }

    if (attackCooldown > 0.0f) attackCooldown -= time;
}
