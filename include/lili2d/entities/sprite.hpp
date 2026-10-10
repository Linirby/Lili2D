#pragma once

#include "lili2d/ecs/ecs_registry.hpp"
#include "lili2d/ecs/entity.hpp"
#include "lili2d/geometry/vec2.hpp"
#include "lili2d/geometry/vec4.hpp"
#include "lili2d/render/common/atlas_map.hpp"
#include "lili2d/render/gpu/pass_types.hpp"
#include <cstdint>

namespace lili {

class Renderer;

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
