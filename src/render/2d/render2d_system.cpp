#include "lili2d/render/2d/render2d_system.hpp"

#include "lili2d/components/circle.hpp"
#include "lili2d/components/line.hpp"
#include "lili2d/components/rect.hpp"
#include "lili2d/components/render2d.hpp"
#include "lili2d/components/sprite.hpp"
#include "lili2d/components/transform.hpp"
#include "lili2d/ecs/ecs_registry.hpp"
#include "lili2d/ecs/ecs_view.hpp"
#include "lili2d/render/2d/dynamic_batch.hpp"
#include "lili2d/render/common/atlas_map.hpp"
#include "lili2d/render/gpu/texture.hpp"
#include <algorithm>
#include <cstdint>
#include <limits>
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

    size_t max_items = registry.getPool<SpriteComponent>().size() +
                       registry.getPool<RectComponent>().size() +
                       registry.getPool<CircleComponent>().size() +
                       registry.getPool<LineComponent>().size();
    if (items.capacity() < max_items)
        items.reserve(max_items);

    pushSprites(registry);
    pushRects(registry, the_white_pixel);
    pushCircles(registry, the_white_pixel);
    pushLines(registry, the_white_pixel);

    current_layer = std::numeric_limits<float>::min();
    current_render_pass = RenderLayer::UNDEFINE;
    current_material_id = 0;
    current_texture = nullptr;

    if (!items.empty()) {
        item_indices.resize(items.size());
        std::iota(item_indices.begin(), item_indices.end(), 0);
        std::sort(item_indices.begin(), item_indices.end(), compareIndex);

        for (uint32_t idx : item_indices) {
            const DrawItem& item = items[idx];
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
                             .width = item.size.x,
                             .height = item.size.y },
                    item.pos,
                    item.nature.sprite.scale,
                    item.rotation,
                    item.color
                );
            else if (item.type == DrawType::RECT)
                batch.addRect(
                    item.pos,
                    item.size,
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
Render2DSystem::pushSprites(ECSRegistry& registry)
{
    auto view =
        registry.view<Render2DComponent, TransformComponent, SpriteComponent>();

    view.each([](Entity entity,
                 Render2DComponent& render2d,
                 TransformComponent& trans,
                 SpriteComponent& sprite) {
        if (!render2d.is_visible)
            return;
        items.push_back(
            DrawItem{
                .texture = sprite.slice.texture,
                .color = render2d.color,
                .pos = trans.pos,
                .size = render2d.size,
                .layer = render2d.layer,
                .rotation = trans.rotation,
                .entity = entity,
                .material_id = render2d.material_id,
                .render_pass = render2d.render_pass,

                .type = DrawType::SPRITE,
                .nature{ .sprite{ .scale = trans.scale,
                                  .u_min = sprite.slice.u_min,
                                  .v_min = sprite.slice.v_min,
                                  .u_max = sprite.slice.u_max,
                                  .v_max = sprite.slice.v_max } },
            }
        );
    });
}

void
Render2DSystem::pushRects(ECSRegistry& registry, Texture* the_white_pixel)
{
    auto rect_view =
        registry.view<Render2DComponent, TransformComponent, RectComponent>();

    rect_view.each([the_white_pixel](
                       Entity entity,
                       Render2DComponent& render2d,
                       TransformComponent& trans,
                       RectComponent& rect
                   ) {
        if (!render2d.is_visible)
            return;
        items.push_back(
            DrawItem{
                .texture = the_white_pixel,
                .color = render2d.color,
                .pos = trans.pos,
                .size = render2d.size,
                .layer = render2d.layer,
                .rotation = trans.rotation,
                .entity = entity,
                .material_id = render2d.material_id,
                .render_pass = render2d.render_pass,

                .type = DrawType::RECT,
                .nature{ .rect{ .scale = trans.scale,
                                .thickness = rect.thickness,
                                .hollow = rect.hollow } },
            }
        );
    });
}

void
Render2DSystem::pushCircles(ECSRegistry& registry, Texture* the_white_pixel)
{
    auto circle_view =
        registry.view<Render2DComponent, TransformComponent, CircleComponent>();

    circle_view.each([the_white_pixel](
                         Entity entity,
                         Render2DComponent& render2d,
                         TransformComponent& trans,
                         CircleComponent& circle
                     ) {
        if (!render2d.is_visible)
            return;
        items.push_back(
            DrawItem{
                .texture = the_white_pixel,
                .color = render2d.color,
                .pos = trans.pos,
                .layer = render2d.layer,
                .rotation = trans.rotation,
                .entity = entity,
                .material_id = render2d.material_id,
                .render_pass = render2d.render_pass,

                .type = DrawType::CIRCLE,
                .nature{ .circle{ .radius = circle.radius,
                                  .thickness = circle.thickness,
                                  .segments = circle.segments,
                                  .hollow = circle.hollow } },
            }
        );
    });
}

void
Render2DSystem::pushLines(ECSRegistry& registry, Texture* the_white_pixel)
{
    auto line_view =
        registry.view<Render2DComponent, TransformComponent, LineComponent>();

    line_view.each([the_white_pixel](
                       Entity entity,
                       Render2DComponent& render2d,
                       TransformComponent& trans,
                       LineComponent& line
                   ) {
        if (!render2d.is_visible)
            return;
        items.push_back(
            DrawItem{
                .texture = the_white_pixel,
                .color = render2d.color,
                .pos = trans.pos,
                .layer = render2d.layer,
                .rotation = trans.rotation,
                .entity = entity,
                .material_id = render2d.material_id,
                .render_pass = render2d.render_pass,

                .type = DrawType::LINE,
                .nature{
                    .line{ .dxy = line.dxy, .thickness = line.thickness } },
            }
        );
    });
}

} // namespace lili
