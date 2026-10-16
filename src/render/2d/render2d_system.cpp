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
#include "lili2d/render/common/atlas_map.hpp"
#include "lili2d/render/gpu/texture.hpp"
#include <algorithm>
#include <cstdint>
#include <numeric>

namespace lili {

void
Render2DSystem::render(
    ECSRegistry& registry,
    DynamicBatch& batch,
    Texture* the_white_pixel
)
{
    items.clear();
    pushSprites(registry, batch);
    pushRects(registry, batch, the_white_pixel);
    pushCircles(registry, batch, the_white_pixel);
    pushLines(registry, batch, the_white_pixel);

    if (!items.empty()) {
        item_indices.resize(items.size());
        std::iota(item_indices.begin(), item_indices.end(), 0);
        std::sort(item_indices.begin(), item_indices.end(), compareIndex);

        for (uint32_t idx : item_indices) {
            DrawItem item = items[idx];
            if (shouldFlush(item)) {
                batch.flush(current_layer, current_render_pass);
                current_render_pass = item.render_pass;
                current_layer = item.layer;
                current_material_id = item.material_id;
                current_texture = item.texture;
            }

            if (item.type == DrawType::SPRITE)
                batch.addSprite(
                    SliceUV{ .texture = item.texture,
                             .u_min = item.nature.sprite.u_min,
                             .v_min = item.nature.sprite.v_min,
                             .u_max = item.nature.sprite.u_max,
                             .v_max = item.nature.sprite.v_max,
                             .width = item.nature.sprite.size.x,
                             .height = item.nature.sprite.size.y },
                    item.pos,
                    item.nature.sprite.scale,
                    item.rotation,
                    item.color
                );
            else if (item.type == DrawType::RECT)
                batch.addRect(
                    item.pos,
                    item.nature.rect.size,
                    item.nature.rect.scale,
                    item.rotation,
                    item.color,
                    item.nature.rect.hollow,
                    item.nature.rect.thickness
                );
            else if (item.type == DrawType::CIRCLE)
                batch.addCircle(
                    item.pos,
                    item.nature.circle.radius,
                    item.nature.circle.segments,
                    item.color,
                    item.nature.circle.hollow,
                    item.nature.circle.thickness
                );
            else if (item.type == DrawType::LINE)
                batch.addLine(
                    item.pos,
                    item.pos + item.nature.line.dxy,
                    item.nature.line.thickness,
                    item.color
                );
        }
    }
    batch.flush(current_layer, current_render_pass);
}

void
Render2DSystem::pushSprites(ECSRegistry& registry, DynamicBatch& batch)
{
    auto view =
        registry.view<Render2DComponent, TransformComponent, SpriteComponent>();

    for (auto&& [entity, render2d, trans, sprite] : view) {
        if (!render2d.is_visible)
            continue;
        items.push_back(
            DrawItem{
                .texture = sprite.slice.texture,
                .color = sprite.tint,
                .pos = trans.pos,
                .layer = render2d.layer,
                .rotation = trans.rotation,
                .entity = entity,
                .material_id = sprite.material_id,
                .render_pass = render2d.render_pass,

                .type = DrawType::SPRITE,
                .nature{ .sprite{
                    .size = { sprite.slice.width, sprite.slice.height },
                    .scale = trans.scale,
                    .u_min = sprite.slice.u_min,
                    .v_min = sprite.slice.v_min,
                    .u_max = sprite.slice.u_max,
                    .v_max = sprite.slice.v_max } },
            }
        );
    }
}

void
Render2DSystem::pushRects(
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
        items.push_back(
            DrawItem{
                .texture = the_white_pixel,
                .color = rect.color,
                .pos = trans.pos,
                .layer = render2d.layer,
                .rotation = trans.rotation,
                .entity = entity,
                .material_id = rect.material_id,
                .render_pass = render2d.render_pass,

                .type = DrawType::RECT,
                .nature{ .rect{ .size = rect.size,
                                .scale = trans.scale,
                                .thickness = rect.thickness,
                                .hollow = rect.hollow } },
            }
        );
    }
}

void
Render2DSystem::pushCircles(
    ECSRegistry& registry,
    DynamicBatch& batch,
    Texture* the_white_pixel
)
{
    auto circle_view =
        registry.view<Render2DComponent, TransformComponent, CircleComponent>();

    for (auto&& [entity, render2d, trans, circle] : circle_view) {
        if (!render2d.is_visible)
            continue;
        items.push_back(
            DrawItem{
                .texture = the_white_pixel,
                .color = circle.color,
                .pos = trans.pos,
                .layer = render2d.layer,
                .rotation = trans.rotation,
                .entity = entity,
                .material_id = circle.material_id,
                .render_pass = render2d.render_pass,

                .type = DrawType::CIRCLE,
                .nature{ .circle{ .radius = circle.radius,
                                  .thickness = circle.thickness,
                                  .segments = circle.segments,
                                  .hollow = circle.hollow } },
            }
        );
    }
}

void
Render2DSystem::pushLines(
    ECSRegistry& registry,
    DynamicBatch& batch,
    Texture* the_white_pixel
)
{
    auto line_view =
        registry.view<Render2DComponent, TransformComponent, LineComponent>();

    for (auto&& [entity, render2d, trans, line] : line_view) {
        if (!render2d.is_visible)
            continue;
        items.push_back(
            DrawItem{
                .texture = the_white_pixel,
                .color = line.color,
                .pos = trans.pos,
                .layer = render2d.layer,
                .rotation = trans.rotation,
                .entity = entity,
                .material_id = line.material_id,
                .render_pass = render2d.render_pass,

                .type = DrawType::LINE,
                .nature{
                    .line{ .dxy = line.dxy, .thickness = line.thickness } },
            }
        );
    }
}

} // namespace lili
