#pragma once

#include <cstdint>
#include <type_traits>

namespace lili {

struct AnimationComponent
{
    float frame_time = 0.1f;
    float frame_timer = 0.0f;
    uint32_t current_frame = 0;
    uint16_t animation_id = 0;
};

static_assert(
    std::is_trivially_copyable_v<AnimationComponent>,
    "AnimationComponent must be trivially copyable"
);

} // namespace lili
