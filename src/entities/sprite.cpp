#include "lili2d/entities/sprite.hpp"
#include "lili2d/components/render2d.hpp"
#include "lili2d/components/sprite.hpp"
#include "lili2d/components/transform.hpp"
#include "lili2d/core/asset_manager.hpp"
#include "lili2d/render/common/atlas_map.hpp"
#include "lili2d/render/gpu/texture.hpp"
#include "lili2d/render/renderer.hpp"
#include <stdexcept>

namespace lili {

[[nodiscard]] Entity
createSprite(
    ECSRegistry& registry,
    Renderer* renderer,
    const std::string& img_path,
    Vec2 pos,
    Vec2 scale,
    float rotation,
    Vec4 color_tint,
    float layer,
    RenderLayer render_pass,
    bool is_visible,
    uint16_t material_id
)
{
    Entity entity = registry.createEntity();
    AssetManager& assets = AssetManager::get();
    Texture* texture = assets.loadTexture(img_path, renderer->getDevice());
    if (!texture)
        throw std::runtime_error("Failed to load texture " + img_path);

    TransformComponent transform{
        .pos = pos, .prev_pos = pos, .scale = scale, .rotation = rotation
    };
    registry.emplaceComponent<TransformComponent>(entity, transform);

    Render2DComponent render2d{
        .color = color_tint,
        .size = Vec2(texture->getWidth(), texture->getHeight()),
        .layer = layer,
        .material_id = material_id,
        .render_pass = render_pass,
        .is_visible = is_visible
    };
    registry.emplaceComponent<Render2DComponent>(entity, render2d);

    SpriteComponent sprite{ .slice = {
                                texture,
                                0.0f,
                                0.0f,
                                1.0f,
                                1.0f,
                                static_cast<float>(texture->getWidth()),
                                static_cast<float>(texture->getHeight()) } };
    registry.emplaceComponent<SpriteComponent>(entity, sprite);

    return entity;
}

[[nodiscard]] Entity
createSprite(
    ECSRegistry& registry,
    const SliceUV& sliceUV,
    Vec2 pos,
    Vec2 scale,
    float rotation,
    Vec4 color_tint,
    float layer,
    RenderLayer render_pass,
    bool is_visible,
    uint16_t material_id
)
{
    Entity entity = registry.createEntity();

    TransformComponent transform{
        .pos = pos, .prev_pos = pos, .scale = scale, .rotation = rotation
    };
    registry.emplaceComponent<TransformComponent>(entity, transform);

    Render2DComponent render2d{ .color = color_tint,
                                .size = { sliceUV.width, sliceUV.height },
                                .layer = layer,
                                .material_id = material_id,
                                .render_pass = render_pass,
                                .is_visible = is_visible };
    registry.emplaceComponent<Render2DComponent>(entity, render2d);

    SpriteComponent sprite{ .slice = sliceUV };
    registry.emplaceComponent<SpriteComponent>(entity, sprite);

    return entity;
}

} // namespace lili
