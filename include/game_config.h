#pragma once

namespace keyless::config {

constexpr float PLAYER_MOVE_SPEED = 0.1f;
constexpr float PLAYER_CLIMB_SPEED = 0.1f;
constexpr float PLAYER_JUMP_SPEED = 0.4f;
constexpr float GRAVITY = 0.0015f;
constexpr float PLAYER_ATTACK_COOLDOWN_MS = 1700.0f;
constexpr float PLAYER_DAMAGE_COOLDOWN_MS = 260.0f;
constexpr float PLAYER_DAMAGE_FLASH_MS = 180.0f;
constexpr int PLAYER_BASE_ATTACK_DAMAGE = 50;
constexpr int PLAYER_POWER_ATTACK_DAMAGE = 100;
constexpr int PLAYER_MAX_HEALTH = 100;
constexpr int SPIKE_DAMAGE = 25;

constexpr float ENEMY_MOVE_SPEED = 0.005f;
constexpr float ENEMY_ATTACK_COOLDOWN_MS = 4000.0f;
constexpr float ENEMY_DAMAGE_FLASH_MS = 180.0f;
constexpr int ENEMY_ATTACK_DAMAGE = 10;

} // namespace keyless::config
