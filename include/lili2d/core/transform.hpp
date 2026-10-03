#pragma once

#include "lili2d/geometry/vec2.hpp"
#include <type_traits>

namespace lili {

struct TransformComponent
{
    lili::Vec2 pos = { 0.0f, 0.0f };
    lili::Vec2 prev_pos = pos;
    lili::Vec2 scale = { 1.0f, 1.0f };
    float rotation = 0.0f;
};

static_assert(
    std::is_trivially_copyable_v<TransformComponent>,
    "TransformComponent must be trivially copyable"
);

} // namespace lili
