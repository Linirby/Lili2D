#pragma once

#include <cstdint>
#include <type_traits>

namespace lili {

/// @brief Component storing playback state and registry ID for a 2D animation.
struct cAnimation
{
    float frame_time = 0.1f;    ///< Duration of each frame in seconds.
    float frame_timer = 0.0f;   ///< Elapsed time on the current frame.
    uint32_t current_frame = 0; ///< Active frame index in the animation.
    uint16_t animation_id =
        0; ///< Registered animation ID in AnimationRegistry.
};

static_assert(
    std::is_trivially_copyable_v<cAnimation>,
    "AnimationComponent must be trivially copyable"
);

} // namespace lili
