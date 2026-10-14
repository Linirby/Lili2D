#include "lili2d/render/2d/render2d_system.hpp"

#include "lili2d/core/transform.hpp"
#include "lili2d/ecs/ecs_registry.hpp"
#include "lili2d/ecs/ecs_view.hpp"
#include "lili2d/render/2d/circle.hpp"
#include "lili2d/render/2d/dynamic_batch.hpp"
#include "lili2d/render/2d/line.hpp"
#include "lili2d/render/2d/rect.hpp"
#include "lili2d/render/2d/render2d_component.hpp"
#include "lili2d/render/2d/sprite.hpp"
#include "lili2d/render/gpu/texture.hpp"
#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <numeric>

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
            SpriteDrawItem{ .entity = entity,
                            .render_pass = render2d.render_pass,
                            .layer = render2d.layer,
                            .material_id = sprite.material_id,
                            .slice = sprite.slice,
                            .pos = trans.pos,
                            .scale = trans.scale,
                            .rotation = trans.rotation,
                            .tint = sprite.tint }
        );
    }
    sprite_indices.resize(sprites.size());
    std::iota(sprite_indices.begin(), sprite_indices.end(), 0);
    std::sort(
        sprite_indices.begin(),
        sprite_indices.end(),
        [](size_t a, size_t b) -> bool {
            const SpriteDrawItem& item_a = sprites[a];
            const SpriteDrawItem& item_b = sprites[b];

            if (item_a.render_pass != item_b.render_pass)
                return item_a.render_pass < item_b.render_pass;
            if (item_a.layer != item_b.layer)
                return item_a.layer < item_b.layer;
            if (item_a.material_id != item_b.material_id)
                return item_a.material_id < item_b.material_id;
            if (item_a.slice.texture != item_b.slice.texture)
                return item_a.slice.texture < item_b.slice.texture;
            return item_a.entity < item_b.entity;
        }
    );

    for (uint32_t idx : sprite_indices) {
        SpriteDrawItem item = sprites[idx];

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
Render2DSystem::renderRects(
    ECSRegistry& registry,
    DynamicBatch& batch,
    Texture* the_white_pixel
)
{
    auto rect_view =
        registry.view<Render2DComponent, TransformComponent, RectComponent>();

    rects.clear();
    for (auto&& [entity, render2d, trans, rect] : rect_view) {
        if (!render2d.is_visible)
            continue;
        rects.push_back(
            RectDrawItem{ .entity = entity,
                          .render_pass = render2d.render_pass,
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
    rect_indices.resize(rects.size());
    std::iota(rect_indices.begin(), rect_indices.end(), 0);
    std::sort(
        rect_indices.begin(),
        rect_indices.end(),
        [](uint32_t a, uint32_t b) -> bool {
            RectDrawItem item_a = rects[a];
            RectDrawItem item_b = rects[b];

            if (item_a.render_pass != item_b.render_pass)
                return item_a.render_pass < item_b.render_pass;
            if (item_a.layer != item_b.layer)
                return item_a.layer < item_b.layer;
            if (item_a.material_id != item_b.material_id)
                return item_a.material_id < item_b.material_id;
            return item_a.entity < item_b.entity;
        }
    );

    for (auto idx : rect_indices) {
        RectDrawItem item = rects[idx];

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
}

void
Render2DSystem::renderCircles(
    ECSRegistry& registry,
    DynamicBatch& batch,
    Texture* the_white_pixel
)
{
    auto circle_view =
        registry.view<Render2DComponent, TransformComponent, CircleComponent>();

    circles.clear();
    for (auto&& [entity, render2d, trans, circle] : circle_view) {
        if (!render2d.is_visible)
            continue;
        circles.push_back(
            CircleDrawItem{ .entity = entity,
                            .render_pass = render2d.render_pass,
                            .layer = render2d.layer,
                            .material_id = circle.material_id,
                            .pos = trans.pos,
                            .radius = circle.radius,
                            .segments = circle.segments,
                            .color = circle.color,
                            .hollow = circle.hollow,
                            .thickness = circle.thickness }
        );
    }
    circle_indices.resize(circles.size());
    std::iota(circle_indices.begin(), circle_indices.end(), 0);
    std::sort(
        circle_indices.begin(),
        circle_indices.end(),
        [](uint32_t a, uint32_t b) -> bool {
            CircleDrawItem item_a = circles[a];
            CircleDrawItem item_b = circles[b];

            if (item_a.render_pass != item_b.render_pass)
                return item_a.render_pass < item_b.render_pass;
            if (item_a.layer != item_b.layer)
                return item_a.layer < item_b.layer;
            if (item_a.material_id != item_b.material_id)
                return item_a.material_id < item_b.material_id;
            return item_a.entity < item_b.entity;
        }
    );

    for (auto idx : circle_indices) {
        CircleDrawItem item = circles[idx];

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
        batch.addCircle(
            item.pos,
            item.radius,
            item.segments,
            item.color,
            item.hollow,
            item.thickness
        );
    }
    batch.flush(current_layer, current_render_pass);
}

void
Render2DSystem::renderLines(
    ECSRegistry& registry,
    DynamicBatch& batch,
    Texture* the_white_pixel
)
{
    auto line_view =
        registry.view<Render2DComponent, TransformComponent, LineComponent>();

    lines.clear();
    for (auto&& [entity, render2d, trans, line] : line_view) {
        if (!render2d.is_visible)
            continue;
        lines.push_back(
            LineDrawItem{ .entity = entity,
                          .render_pass = render2d.render_pass,
                          .layer = render2d.layer,
                          .material_id = line.material_id,
                          .start_pos = trans.pos,
                          .dxy = line.dxy,
                          .thickness = line.thickness,
                          .color = line.color }
        );
    }
    circle_indices.resize(lines.size());
    std::iota(circle_indices.begin(), circle_indices.end(), 0);
    std::sort(
        circle_indices.begin(),
        circle_indices.end(),
        [](uint32_t a, uint32_t b) -> bool {
            LineDrawItem item_a = lines[a];
            LineDrawItem item_b = lines[b];

            if (item_a.render_pass != item_b.render_pass)
                return item_a.render_pass < item_b.render_pass;
            if (item_a.layer != item_b.layer)
                return item_a.layer < item_b.layer;
            if (item_a.material_id != item_b.material_id)
                return item_a.material_id < item_b.material_id;
            return item_a.entity < item_b.entity;
        }
    );

    for (auto idx : circle_indices) {
        LineDrawItem item = lines[idx];

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
        batch.addLine(item.start_pos, item.dxy, item.thickness, item.color);
    }
    batch.flush(current_layer, current_render_pass);
}

} // namespace lili
