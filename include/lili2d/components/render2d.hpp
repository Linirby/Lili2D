#pragma once

#include "lili2d/geometry/vec2.hpp"
#include "lili2d/geometry/vec4.hpp"
#include "lili2d/render/gpu/pass_types.hpp"
#include <cstdint>
#include <type_traits>

namespace lili {

/// @brief Component holding shared 2D rendering properties (color, quad size,
/// layer, material, render pass, and visibility).
struct Render2DComponent
{
    Vec4 color = { 1.0f, 1.0f, 1.0f, 1.0f }; ///< Color or tint RGBA.
    Vec2 size = { 0.0f, 0.0f };              ///< Quad size in pixels (used by Sprite & Rect, and UI bounding box).
    float layer = 0.0f;                      ///< Depth sorting layer.
    uint16_t material_id = 0;                ///< Linked material ID.
    RenderLayer render_pass = RenderLayer::WORLD2D; ///< Target render pass (WORLD2D, UI, etc.).
    bool is_visible = true;                  ///< Whether this entity should be rendered.
};

static_assert(
    std::is_trivially_copyable_v<Render2DComponent>,
    "Render2DComponent must be trivially copyable"
);

} // namespace lili
