#include "app.hpp"
#include "lili2d/core/game.hpp"
#include "lili2d/ecs/ecs_registry.hpp"
#include "lili2d/entities/sprite.hpp"
#include "lili2d/geometry/vec2.hpp"
#include "lili2d/render/2d/ui_layout.hpp"
#include "lili2d/render/2d/ui_layout_system.hpp"
#include "lili2d/render/renderer.hpp"
#include <memory>

void
App::onInit()
{
    lili::ECSRegistry& registry = getECSRegistry();
    lili::Renderer* renderer = getRenderer();

    lili::Vec2 pos = { 400.0f, 400.0f };
    lili::Vec2 scale = { 0.5f, 0.5f };
    float rotation = 45.0f;
    cat_img = lili::createSprite(
        registry, renderer, "baguette.jpg", pos, scale, rotation
    );
    registry.addComponents(
        cat_img,
        lili::UILayoutComponent{ .anchor = lili::Anchor::CENTER,
                                 .pivot = lili::Pivot::CENTER }
    );

    batch = std::make_unique<lili::DynamicBatch>(renderer, nullptr);
}

void
App::onEvent(const lili::Event& event)
{
    if (event.type() == lili::EventType::KEYBOARD) {
        lili::KeyboardEvent kb = event.keyboard();
        if (event.keyJustPressed(kb) && kb.key == lili::Key::ESCAPE)
            shutdown();
    }
}

void
App::onUpdate(float dt)
{
    lili::UILayoutSystem::update(getECSRegistry(), getWindow()->getSize());
}

void
App::onRender([[maybe_unused]] float alpha)
{
    lili::Game::onRender(alpha);
}
