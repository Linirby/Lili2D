#include "lili2d/render/2d/render2d_system.hpp"

#include "lili2d/core/transform.hpp"
#include "lili2d/ecs/ecs_view.hpp"
#include "lili2d/render/2d/render2d_component.hpp"
#include "lili2d/render/2d/sprite.hpp"
#include "lili2d/render/2d/sprite_batch.hpp"
#include "lili2d/render/gpu/pass_types.hpp"
#include "lili2d/render/gpu/texture.hpp"
#include <algorithm>
#include <cstdint>
#include <limits>

namespace lili {

void
Render2DSystem::renderSprites(ECSRegistry& registry, SpriteBatch& batch)
{
    auto view =
        registry.view<Render2DComponent, TransformComponent, SpriteComponent>();

    sprite_items.clear();
    for (auto&& [entity, render2d, trans, sprite] : view) {
        if (!render2d.is_visible)
            continue;
        sprite_items.push_back(
            SpriteDrawItem{ .render_pass = render2d.render_pass,
                            .layer = render2d.layer,
                            .material_id = sprite.material_id,
                            .slice = sprite.slice,
                            .pos = trans.pos,
                            .scale = trans.scale,
                            .rotation = trans.rotation,
                            .tint = sprite.tint }
        );
    }
    std::sort(
        sprite_items.begin(),
        sprite_items.end(),
        [](const SpriteDrawItem& a, const SpriteDrawItem& b) -> bool {
            if (a.render_pass != b.render_pass)
                return a.render_pass < b.render_pass;
            if (a.layer != b.layer)
                return a.layer < b.layer;
            if (a.material_id != b.material_id)
                return a.material_id < b.material_id;
            return a.slice.texture < b.slice.texture;
        }
    );

    batch.begin();

    RenderLayer current_render_pass = RenderLayer::UNDEFINE;
    float current_layer = std::numeric_limits<float>::min();
    uint16_t current_material_id = 0;
    Texture* current_texture = nullptr;

    for (auto&& item : sprite_items) {
        bool should_flush = current_render_pass != item.render_pass ||
                            current_layer != item.layer ||
                            current_material_id != item.material_id ||
                            current_texture != item.slice.texture;
        if (should_flush) {
            batch.flush(current_layer, current_render_pass);
            current_render_pass = item.render_pass;
            current_layer = item.layer;
            current_material_id = item.material_id;
            current_texture = item.slice.texture;
        }
        batch.add(item.slice, item.pos, item.scale, item.rotation, item.tint);
    }
    batch.flush(current_layer, current_render_pass);
}

} // namespace lili
