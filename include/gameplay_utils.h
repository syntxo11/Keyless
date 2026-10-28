#pragma once

#include <algorithm>
#include <cmath>

namespace keyless::gameplay {

constexpr float PI = 3.14159265358979323846f;
constexpr float IDLE_BREATH_AMPLITUDE = 1.0f;
constexpr float IDLE_BREATH_SPEED = 0.0025f;
constexpr float ENEMY_MELEE_GAP = 24.0f;
constexpr float ENEMY_AGGRO_DISTANCE = 110.0f;

inline float advanceIdlePhase(float phase, float frameTime) {
    phase += IDLE_BREATH_SPEED * std::max(0.0f, frameTime);
    const float fullCycle = 2.0f * PI;
    if (phase >= fullCycle) {
        phase = std::fmod(phase, fullCycle);
    }
    return phase;
}

inline float idleBreathOffset(float phase) {
    // Round to whole pixels so the pixel-art sprite stays crisp.
    return std::round(std::sin(phase) * IDLE_BREATH_AMPLITUDE);
}

inline float horizontalGap(float leftA, float widthA, float leftB, float widthB) {
    const float rightA = leftA + widthA;
    const float rightB = leftB + widthB;

    if (rightA < leftB) return leftB - rightA;
    if (rightB < leftA) return leftA - rightB;
    return 0.0f;
}

inline bool rangesOverlap(float startA, float sizeA, float startB, float sizeB) {
    return startA < startB + sizeB && startB < startA + sizeA;
}

} // namespace keyless::gameplay
