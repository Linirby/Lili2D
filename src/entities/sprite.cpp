#include "lili2d/entities/sprite.hpp"
#include "lili2d/core/asset_manager.hpp"
#include "lili2d/core/transform.hpp"
#include "lili2d/render/2d/render2d_component.hpp"
#include "lili2d/render/2d/sprite.hpp"
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

    TransformComponent transform{};
    transform.pos = pos;
    transform.prev_pos = pos;
    transform.scale = scale;
    transform.rotation = rotation;
    registry.emplaceComponent<TransformComponent>(entity, transform);

    Render2DComponent render2d{};
    render2d.size = Vec2(texture->getWidth(), texture->getHeight());
    render2d.layer = layer;
    render2d.is_visible = is_visible;
    render2d.render_pass = render_pass;
    registry.emplaceComponent<Render2DComponent>(entity, render2d);

    SpriteComponent sprite{};
    sprite.slice = { texture,
                     0.0f,
                     0.0f,
                     1.0f,
                     1.0f,
                     static_cast<float>(texture->getWidth()),
                     static_cast<float>(texture->getHeight()) };
    sprite.material_id = material_id;
    sprite.tint = color_tint;
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

    Render2DComponent render2d{ .size = { sliceUV.width, sliceUV.height },
                                .layer = layer,
                                .is_visible = is_visible,
                                .render_pass = render_pass };
    registry.emplaceComponent<Render2DComponent>(entity, render2d);

    SpriteComponent sprite{ .slice = sliceUV,
                            .material_id = material_id,
                            .tint = color_tint };
    registry.emplaceComponent<SpriteComponent>(entity, sprite);

    return entity;
}

} // namespace lili
