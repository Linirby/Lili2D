#pragma once

#include "lili2d/ecs/ecs_registry.hpp"
#include "lili2d/ecs/entity.hpp"
#include "lili2d/geometry/vec2.hpp"
#include "lili2d/geometry/vec4.hpp"
#include "lili2d/render/common/atlas_map.hpp"
#include "lili2d/render/gpu/pass_types.hpp"
#include <cstdint>
#include <string>

namespace lili {

class Renderer;

/// @brief Creates a 2D Sprite entity from an image file path.
/// @note Attached components:
/// - `TransformComponent` (`pos`, `prev_pos`, `scale`, `rotation`)
/// - `Render2DComponent` (`color`, `size`, `layer`, `material_id`, `render_pass`, `is_visible`)
/// - `SpriteComponent` (`slice`)
/// @param registry The ECS registry.
/// @param renderer Pointer to the Renderer.
/// @param img_path Path to the texture image file.
/// @param pos World or screen position in pixels.
/// @param scale Scale factor in X and Y.
/// @param rotation Rotation in degrees.
/// @param color_tint RGBA color tint.
/// @param layer Z-ordering depth layer.
/// @param render_pass Target render pass (`WORLD2D` or `UI`).
/// @param is_visible Initial visibility flag.
/// @param material_id Linked material ID.
/// @return The created Entity.
[[nodiscard]] Entity
createSprite(
    ECSRegistry& registry,
    Renderer* renderer,
    const std::string& img_path,
    Vec2 pos,
    Vec2 scale = { 1.0f, 1.0f },
    float rotation = 0.0f,
    Vec4 color_tint = { 1.0f, 1.0f, 1.0f, 1.0f },
    float layer = 0.0f,
    RenderLayer render_pass = RenderLayer::WORLD2D,
    bool is_visible = true,
    uint16_t material_id = 0
);

/// @brief Creates a 2D Sprite entity from an existing UV slice.
/// @note Attached components:
/// - `TransformComponent` (`pos`, `prev_pos`, `scale`, `rotation`)
/// - `Render2DComponent` (`color`, `size`, `layer`, `material_id`, `render_pass`, `is_visible`)
/// - `SpriteComponent` (`slice`)
/// @param registry The ECS registry.
/// @param sliceUV The UV slice and texture pointer.
/// @param pos World or screen position in pixels.
/// @param scale Scale factor in X and Y.
/// @param rotation Rotation in degrees.
/// @param color_tint RGBA color tint.
/// @param layer Z-ordering depth layer.
/// @param render_pass Target render pass (`WORLD2D` or `UI`).
/// @param is_visible Initial visibility flag.
/// @param material_id Linked material ID.
/// @return The created Entity.
[[nodiscard]] Entity
createSprite(
    ECSRegistry& registry,
    const SliceUV& sliceUV,
    Vec2 pos,
    Vec2 scale = { 1.0f, 1.0f },
    float rotation = 0.0f,
    Vec4 color_tint = { 1.0f, 1.0f, 1.0f, 1.0f },
    float layer = 0.0f,
    RenderLayer render_pass = RenderLayer::WORLD2D,
    bool is_visible = true,
    uint16_t material_id = 0
);

} // namespace lili
