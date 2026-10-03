#pragma once

#include "lili2d/render/gpu/pass_types.hpp"

namespace lili {

struct Render2DComponent
{
    float layer = 0.0f;
    RenderLayer render_pass = RenderLayer::WORLD2D;
    bool is_visible = true;
};

static_assert(
    std::is_trivially_copyable_v<Render2DComponent>,
    "Render2DComponent must be trivially copyable"
);

} // namespace lili
