#include "lili2d/render/2d/render2d_system.hpp"

#include "lili2d/core/transform.hpp"
#include "lili2d/ecs/ecs_registry.hpp"
#include "lili2d/ecs/ecs_view.hpp"
// #include "lili2d/render/2d/circle.hpp"
#include "lili2d/render/2d/dynamic_batch.hpp"
// #include "lili2d/render/2d/line.hpp"
#include "lili2d/render/2d/rect.hpp"
#include "lili2d/render/2d/render2d_component.hpp"
#include "lili2d/render/2d/sprite.hpp"
#include "lili2d/render/gpu/pass_types.hpp"
#include "lili2d/render/gpu/texture.hpp"
#include <algorithm>
#include <cstdint>
#include <limits>

namespace lili {

void
Render2DSystem::renderSprites(ECSRegistry& registry, DynamicBatch& batch)
{
    auto view =
        registry.view<Render2DComponent, TransformComponent, SpriteComponent>();

    sprites.clear();
    for (auto&& [entity, render2d, trans, sprite] : view) {
        if (!render2d.is_visible)
            continue;
        sprites.push_back(
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
        sprites.begin(),
        sprites.end(),
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

    RenderLayer current_render_pass = RenderLayer::UNDEFINE;
    float current_layer = std::numeric_limits<float>::min();
    uint16_t current_material_id = 0;
    Texture* current_texture = nullptr;

    batch.begin();

    for (auto&& item : sprites) {
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
        batch.addSprite(
            item.slice, item.pos, item.scale, item.rotation, item.tint
        );
    }
    batch.flush(current_layer, current_render_pass);
}

void
Render2DSystem::renderShapes(
    ECSRegistry& registry,
    DynamicBatch& batch,
    Texture* the_white_pixel
)
{
    auto rect_view =
        registry.view<Render2DComponent, TransformComponent, RectComponent>();

    for (auto&& [entity, render2d, trans, rect] : rect_view) {
        if (!render2d.is_visible)
            continue;
        rects.push_back(
            RectDrawItem{ .render_pass = render2d.render_pass,
                          .layer = render2d.layer,
                          .material_id = rect.material_id,
                          .pos = trans.pos,
                          .size = rect.size,
                          .scale = trans.scale,
                          .rotation = trans.rotation,
                          .color = rect.color,
                          .hollow = rect.hollow,
                          .thickness = rect.thickness }
        );
    }
    std::sort(
        rects.begin(),
        rects.end(),
        [](const RectDrawItem& a, const RectDrawItem& b) -> bool {
            if (a.render_pass != b.render_pass)
                return a.render_pass < b.render_pass;
            if (a.layer != b.layer)
                return a.layer < b.layer;
            return a.material_id < b.material_id;
        }
    );

    RenderLayer current_render_pass = RenderLayer::UNDEFINE;
    float current_layer = std::numeric_limits<float>::min();
    uint16_t current_material_id = 0;
    Texture* current_texture = nullptr;

    batch.begin();

    for (auto item : rects) {
        bool should_flush = current_render_pass != item.render_pass ||
                            current_layer != item.layer ||
                            current_material_id != item.material_id ||
                            current_texture != the_white_pixel;
        if (should_flush) {
            batch.flush(current_layer, current_render_pass);
            current_render_pass = item.render_pass;
            current_layer = item.layer;
            current_material_id = item.material_id;
            current_texture = the_white_pixel;
        }
        batch.addRect(
            item.pos,
            item.size,
            item.scale,
            item.rotation,
            item.color,
            item.hollow,
            item.thickness
        );
    }
    batch.flush(current_layer, current_render_pass);

    // auto circle_view =
    //     registry.view<Render2DComponent, TransformComponent,
    //     CircleComponent>();

    // auto line_view =
    //     registry.view<Render2DComponent, TransformComponent,
    //     LineComponent>();
}

} // namespace lili
