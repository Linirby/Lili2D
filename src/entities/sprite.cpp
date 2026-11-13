#include "lili2d/entities/sprite.hpp"
#include "lili2d/components/render2d.hpp"
#include "lili2d/components/sprite.hpp"
#include "lili2d/core/asset_manager.hpp"
#include "lili2d/render/common/slice_uv.hpp"
#include "lili2d/render/gpu/texture.hpp"
#include "lili2d/render/renderer.hpp"
#include <stdexcept>

namespace lili {

[[nodiscard]] Entity
createSprite(
    ECSRegistry& registry,
    Renderer* renderer,
    const std::string& img_path,
    SpriteParameters params
)
{
    Entity entity = registry.createEntity();
    AssetManager& assets = AssetManager::get();
    Texture* texture = assets.loadTexture(img_path, renderer->getDevice());
    if (!texture)
        throw std::runtime_error("Failed to load texture \"" + img_path + "\"");

    cTransform transform = params.transform;
    transform.prev_pos = params.transform.pos;

    cRender2D render2d = params.render2d;
    render2d.size = Vec2{
        static_cast<float>(texture->getWidth()),
        static_cast<float>(texture->getHeight()),
    };

    cSprite sprite = params.sprite;
    sprite.slice = SliceUV{
        .texture = texture,
        .width = static_cast<float>(texture->getWidth()),
        .height = static_cast<float>(texture->getHeight()),
    };

    registry.addComponents(entity, transform, render2d, sprite);
    return entity;
}

[[nodiscard]] Entity
createSprite(
    ECSRegistry& registry,
    const SliceUV& sliceUV,
    SpriteParameters params
)
{
    Entity entity = registry.createEntity();

    cRender2D render2d = params.render2d;
    render2d.size = Vec2{
        sliceUV.width,
        sliceUV.height,
    };

    cSprite sprite = params.sprite;
    sprite.slice = sliceUV;

    registry.addComponents(entity, params.transform, render2d, sprite);
    return entity;
}

} // namespace lili
