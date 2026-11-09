#include "app.hpp"

#include <lili2d/core/asset_manager.hpp>
#include <lili2d/entities/animated_sprite.hpp>
#include <lili2d/render/common/animation.hpp>
#include <lili2d/render/common/animation_registry.hpp>

using namespace lili;

App::App()
  : Game("hello_animation - Lili2D", 400, 400)
{
    setTps(20.0f);
    getWindow()->setResizable(true);
}

void
App::onInit()
{
    Assets::setHotReloadEnabled(true);

    Renderer* renderer = getRenderer();
    ECSRegistry& ecs_reg = getECSRegistry();
    AnimationRegistry& anim_reg = AnimationRegistry::get();

    AtlasMap* head_atlas =
        Assets::loadAtlas("head_atlas", renderer, "head_animation.png", 8, 1);
    anim_reg.registerAnimation(
        "head", Animation(head_atlas->getSliceUVs(0, 8))
    );

    head_sprite_old = AnimatedSprite(renderer, anim_reg.getAnimation("head"));
    head_sprite_old.setScale({ 15.0f, 15.0f });
    head_sprite_old.setFrameSpeed(0.2f);
    head_sprite_old.setRender(RenderLayer::UI);
    head_sprite_old.setAnchor(Anchor::BOTTOM);
    head_sprite_old.setPivot(Pivot::BOTTOM);

    head_sprite = createAnimatedSprite(
        ecs_reg,
        "head",
        {
            .transform{ .scale = Vec2(15.0f, 15.0f) },
            .render2d{ .render_pass = lili::RenderLayer::UI },
            .animation{ .frame_time = 0.2f },
        }
    );
    ecs_reg.addComponents(
        head_sprite,
        UILayoutComponent{
            .anchor = lili::Anchor::BOTTOM,
            .pivot = lili::Pivot::BOTTOM,
        }
    );
}

void
App::onEvent(const Event& event)
{
    Game::onEvent(event);

    if (event.type() == EventType::KEYBOARD) {
        KeyboardEvent kb = event.keyboard();
        if (event.keyJustPressed(kb) && kb.key == Key::ESCAPE)
            shutdown();
    }
}

void
App::onUpdate(float dt)
{
    Game::onUpdate(dt);

    head_sprite_old.update(dt);
}

void
App::onRender(float alpha)
{
    Game::onRender(alpha);

    // head_sprite_old.draw();
}
