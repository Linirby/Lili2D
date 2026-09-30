#pragma once

#include "lili2d/geometry/vec2.hpp"

namespace lili {

struct TransformComponent
{
    lili::Vec2 pos = { 0.0f, 0.0f };
    lili::Vec2 prev_pos = pos;
    lili::Vec2 scale = { 1.0f, 1.0f };
    float rotation = 0.0f;
};

static_assert(
    sizeof(TransformComponent) == 28,
    "Transform size must be 28 bytes"
);
static_assert(
    alignof(TransformComponent) == 4,
    "Transform must be 4-byte aligned"
);

} // namespace lili
