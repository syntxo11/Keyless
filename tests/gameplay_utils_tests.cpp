#ifdef NDEBUG
#undef NDEBUG
#endif
#include "gameplay_utils.h"

#include <cassert>
#include <cmath>
#include <iostream>

int main() {
    using namespace keyless::gameplay;

    assert(horizontalGap(0.0f, 10.0f, 20.0f, 5.0f) == 10.0f);
    assert(horizontalGap(20.0f, 5.0f, 0.0f, 10.0f) == 10.0f);
    assert(horizontalGap(0.0f, 10.0f, 5.0f, 10.0f) == 0.0f);

    assert(rangesOverlap(0.0f, 10.0f, 9.0f, 5.0f));
    assert(!rangesOverlap(0.0f, 10.0f, 10.0f, 5.0f));

    float phase = 0.0f;
    for (int i = 0; i < 5000; ++i) {
        phase = advanceIdlePhase(phase, 16.0f);
        const float offset = idleBreathOffset(phase);
        assert(offset >= -IDLE_BREATH_AMPLITUDE);
        assert(offset <= IDLE_BREATH_AMPLITUDE);
        assert(phase >= 0.0f && phase < 2.0f * PI);
    }

    std::cout << "All gameplay utility tests passed.\n";
    return 0;
}
