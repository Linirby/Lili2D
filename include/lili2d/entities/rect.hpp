#pragma once

#include "lili2d/ecs/ecs_registry.hpp"
#include "lili2d/geometry/vec2.hpp"
#include "lili2d/geometry/vec4.hpp"
#include "lili2d/render/gpu/pass_types.hpp"
#include <type_traits>

namespace lili {

struct RectParameters
{
    Vec4 color = { 1.0f, 1.0f, 1.0f, 1.0f };
    Vec2 pos = { 0.0f, 0.0f };
    Vec2 size = { 10.0f, 10.0f };
    Vec2 scale = { 1.0f, 1.0f };
    float rotation = 0.0f;
    float thickness = 1.0f;
    float layer = 0.0f;
    uint16_t material_id = 0;
    bool hollow = false;
    bool is_visible = true;
    RenderLayer render_pass = RenderLayer::WORLD2D;
};

static_assert(
    std::is_trivially_copyable_v<RectParameters>,
    "RectParameters must be tryvially copyable."
);

/// @brief Creates a 2D Rectangle (Quad) entity.
/// @note Attached components:
/// - `TransformComponent` (`pos`, `prev_pos`, `scale`, `rotation`)
/// - `Render2DComponent` (`color`, `size`, `layer`, `material_id`,
/// `render_pass`, `is_visible`)
/// - `RectComponent` (`thickness`, `hollow`)
/// @param registry The ECS registry.
/// @param params The parameters of the rect.
/// @return The created Entity.
[[nodiscard]] Entity
createRect(ECSRegistry& registry, const RectParameters& params);

} // namespace lili
