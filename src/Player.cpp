#include "Player.h"

#include "Enemy.h"
#include "game_config.h"
#include "gameplay_utils.h"
#include "map.h"
#include "view.h"

#include <algorithm>
#include <cmath>
#include <random>

using namespace sf;

Player::Player(Image &image, float X, float Y, int W, int H, String Name)
    : Entity(image, X, Y, W, H, std::move(Name)),
      attackDamage(keyless::config::PLAYER_BASE_ATTACK_DAMAGE) {
    if (name == "Player1") {
        sprite.setTextureRect(IntRect(10, 28, 28, 22));
        sprite.setOrigin(14.0f, 11.0f);
    }
}

FloatRect Player::getHitbox() const {
    return {x + 5.0f, y + 2.0f,
            std::max(1.0f, static_cast<float>(w) - 10.0f),
            std::max(1.0f, static_cast<float>(h) - 3.0f)};
}

FloatRect Player::getAttackHitbox() const {
    const float hitY = y + 1.0f;
    const float hitH = static_cast<float>(h) + 2.0f;
    if (facingRight) {
        return {x + w - 3.0f, hitY, 38.0f, hitH};
    }
    return {x - 35.0f, hitY, 38.0f, hitH};
}

bool Player::takeDamage(int amount) {
    if (!life || damageCooldown > 0.0f) return false;

    health -= amount;
    sprite.setColor(Color::Red);
    damageTimer = keyless::config::PLAYER_DAMAGE_FLASH_MS;
    damageCooldown = keyless::config::PLAYER_DAMAGE_COOLDOWN_MS;

    if (health <= 0) {
        health = 0;
        life = false;
    }
    return true;
}

void Player::setWalkFrame(float time) {
    sprite.setOrigin(14.0f, 11.0f);

    if (!isMove) {
        currentFrame = 0.0f;
        if (facingRight) {
            sprite.setTextureRect(IntRect(10, 28, 28, 22));
        } else {
            // The sprite sheet only has one idle frame. Negative width mirrors it
            // so idle keeps the player's last movement direction.
            sprite.setTextureRect(IntRect(38, 28, -28, 22));
        }
        return;
    }

    currentFrame += 0.012f * time;
    while (currentFrame >= 9.0f) currentFrame -= 9.0f;
    sprite.setTextureRect(IntRect(28 * static_cast<int>(currentFrame),
                                  facingRight ? 0 : 56, 28, 22));
}

void Player::updateIdleBreathing(float time) {
    const bool canBreathe = state == stay && !isMove && !isAttacking && onGround;
    if (!canBreathe) {
        idlePhase = 0.0f;
        idleVisualOffsetY = 0.0f;
        return;
    }

    idlePhase = keyless::gameplay::advanceIdlePhase(idlePhase, time);
    idleVisualOffsetY = keyless::gameplay::idleBreathOffset(idlePhase);
}

void Player::setAttackFrame() {
    const int frame = std::clamp(static_cast<int>(attackFrame), 0, 4);
    if (facingRight) {
        sprite.setOrigin(frame == 0 ? 30.0f : 10.0f, 11.0f);
        sprite.setTextureRect(IntRect(51 * frame, 85, 51, 23));
    } else {
        sprite.setOrigin(frame == 0 ? 22.0f : 40.0f, 11.0f);
        sprite.setTextureRect(IntRect(51 * frame, 111, 51, 23));
    }
}

void Player::control(float time) {
    if (powerUpActive && std::chrono::steady_clock::now() > powerUpEndTime) {
        attackDamage = keyless::config::PLAYER_BASE_ATTACK_DAMAGE;
        powerUpActive = false;
    }

    if (isAttacking) {
        dx = 0.0f;
        isMove = false;
        return;
    }

    state = stay;
    speed = 0.0f;
    dx = 0.0f;
    isMove = false;

    const bool leftPressed = Keyboard::isKeyPressed(Keyboard::Left) || Keyboard::isKeyPressed(Keyboard::A);
    const bool rightPressed = Keyboard::isKeyPressed(Keyboard::Right) || Keyboard::isKeyPressed(Keyboard::D);

    if (leftPressed && !rightPressed) {
        state = left;
        facingRight = false;
        speed = keyless::config::PLAYER_MOVE_SPEED;
        dx = -speed;
        isMove = true;
    } else if (rightPressed && !leftPressed) {
        state = right;
        facingRight = true;
        speed = keyless::config::PLAYER_MOVE_SPEED;
        dx = speed;
        isMove = true;
    }

    if ((Keyboard::isKeyPressed(Keyboard::Up) || Keyboard::isKeyPressed(Keyboard::W)) && canClimb) {
        state = up;
        dy = -keyless::config::PLAYER_CLIMB_SPEED;
        onGround = false;
    }

    if (onGround && Keyboard::isKeyPressed(Keyboard::Space)) {
        state = jump;
        dy = -keyless::config::PLAYER_JUMP_SPEED;
        onGround = false;
    }

    if (Keyboard::isKeyPressed(Keyboard::Down) || Keyboard::isKeyPressed(Keyboard::S)) {
        if (!isMove) state = down;
    }

    setWalkFrame(time);
}

void Player::checkCollisionWithMap(float Dx, float Dy) {
    canClimb = false;

    for (int i = static_cast<int>(std::floor(y / 16.0f));
         i < static_cast<int>(std::ceil((y + h) / 16.0f)); ++i) {
        for (int j = static_cast<int>(std::floor(x / 16.0f));
             j < static_cast<int>(std::ceil((x + w) / 16.0f)); ++j) {
            if (i < 0 || i >= HEIGHT_MAP || j < 0 || j >= WIDTH_MAP) continue;

            const char tile = static_cast<char>(TileMap[i][j]);
            const bool solid = tile == 'a' || tile == 'b' || tile == 'c' || tile == 'd'
                || tile == '{' || tile == '}' || tile == '(' || tile == ')'
                || tile == '[' || tile == ']' || tile == 'M' || tile == 'L' || tile == 'W';

            if (solid) {
                if (Dy > 0.0f) { y = i * 16.0f - h; dy = 0.0f; onGround = true; }
                if (Dy < 0.0f) { y = i * 16.0f + 16.0f; dy = 0.0f; }
                if (Dx > 0.0f) { x = j * 16.0f - w; }
                if (Dx < 0.0f) { x = j * 16.0f + 16.0f; }
            }

            if (tile == 'u' || tile == 'p') {
                canClimb = true;
                onGround = false;
            }

            if (tile == 'i' && (y + h) > (i * 16 + 12)) {
                if (takeDamage(keyless::config::SPIKE_DAMAGE)) {
                    if (Dx > 0.0f) x -= 10.0f;
                    if (Dx < 0.0f) x += 10.0f;
                    if (Dy > 0.0f) y -= 10.0f;
                    if (Dy < 0.0f) y += 10.0f;
                }
            }

            if (tile == 'k' || tile == 'r' || tile == 'l') {
                TileMap[i][j] = ' ';
                ++keys;
            }

            if (tile == 'H') {
                TileMap[i][j] = 'O';
                std::random_device rd;
                std::mt19937 gen(rd());
                std::uniform_int_distribution<> dis(0, 1);
                const char newItem = dis(gen) == 0 ? 'h' : 'f';
                if (j + 1 < WIDTH_MAP) TileMap[i][j + 1] = newItem;
            }

            if (tile == '!' && keys == 1) {
                for (int mapY = 0; mapY < HEIGHT_MAP; ++mapY) {
                    for (int mapX = 0; mapX < WIDTH_MAP; ++mapX) {
                        if (TileMap[mapY][mapX] == '[') TileMap[mapY][mapX] = 'g';
                        if (TileMap[mapY][mapX] == ']') TileMap[mapY][mapX] = '0';
                    }
                }
            } else if (tile == ';' && keys == 2) {
                for (int mapY = 0; mapY < HEIGHT_MAP; ++mapY) {
                    for (int mapX = 0; mapX < WIDTH_MAP; ++mapX) {
                        if (TileMap[mapY][mapX] == '{') TileMap[mapY][mapX] = '/';
                        if (TileMap[mapY][mapX] == '}') TileMap[mapY][mapX] = '|';
                    }
                }
            } else if (tile == ':' && keys == 3) {
                for (int mapY = 0; mapY < HEIGHT_MAP; ++mapY) {
                    for (int mapX = 0; mapX < WIDTH_MAP; ++mapX) {
                        if (TileMap[mapY][mapX] == '(') TileMap[mapY][mapX] = '.';
                        if (TileMap[mapY][mapX] == ')') TileMap[mapY][mapX] = ',';
                    }
                }
            }

            if (tile == 'h') {
                TileMap[i][j] = ' ';
                health = keyless::config::PLAYER_MAX_HEALTH;
            }

            if (tile == 'f') {
                TileMap[i][j] = ' ';
                attackDamage = keyless::config::PLAYER_POWER_ATTACK_DAMAGE;
                powerUpEndTime = std::chrono::steady_clock::now() + std::chrono::seconds(60);
                powerUpActive = true;
            }

            if (tile == 's') {
                showEndScreen = true;
            }
        }
    }
}

void Player::handleAttack(std::vector<std::unique_ptr<Enemy>> &enemies, float time) {
    if (Mouse::isButtonPressed(Mouse::Left) && attackCooldown <= 0.0f && !isAttacking) {
        isAttacking = true;
        attackFrame = 0.0f;
        attackHitDone = false;
        attackCooldown = keyless::config::PLAYER_ATTACK_COOLDOWN_MS;
        dx = 0.0f;
    }

    if (!isAttacking) return;

    attackFrame += 0.018f * time;
    setAttackFrame();

    if (!attackHitDone && attackFrame >= 1.35f) {
        const FloatRect attackBox = getAttackHitbox();
        for (auto &enemy : enemies) {
            if (enemy->life && attackBox.intersects(enemy->getHitbox())) {
                enemy->takeDamage(static_cast<int>(attackDamage));
            }
        }
        attackHitDone = true;
    }

    if (attackFrame >= 5.0f) {
        isAttacking = false;
        attackFrame = 0.0f;
        attackHitDone = false;
        sprite.setOrigin(14.0f, 11.0f);
        setWalkFrame(0.0f);
    }
}

void Player::update(float time, std::vector<std::unique_ptr<Enemy>> &enemies) {
    control(time);

    x += dx * time;
    checkCollisionWithMap(dx, 0.0f);
    y += dy * time;
    checkCollisionWithMap(0.0f, dy);

    if (health <= 0) life = false;
    if (life) getPlayerCoordinateForView(x, y);

    if (damageTimer > 0.0f) {
        damageTimer -= time;
        if (damageTimer <= 0.0f) sprite.setColor(Color::White);
    }
    if (damageCooldown > 0.0f) damageCooldown -= time;
    if (attackCooldown > 0.0f) attackCooldown -= time;

    handleAttack(enemies, time);
    updateIdleBreathing(time);

    // Breathing only changes rendering. Collision coordinates remain unchanged.
    sprite.setPosition(x + w / 2.0f, y + h / 2.0f + idleVisualOffsetY);

    dy += keyless::config::GRAVITY * time;
}
