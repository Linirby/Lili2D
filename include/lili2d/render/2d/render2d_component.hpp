#pragma once

#include "lili2d/render/gpu/pass_types.hpp"

namespace lili {

struct Render2DComponent
{
    Vec2 size;
    float layer = 0.0f;
    bool is_visible = true;
    RenderLayer render_pass = RenderLayer::WORLD2D;
};

static_assert(
    std::is_trivially_copyable_v<Render2DComponent>,
    "Render2DComponent must be trivially copyable"
);

} // namespace lili
