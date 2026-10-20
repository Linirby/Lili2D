#include "app.hpp"

using namespace lili;

void
App::onInit()
{
    ECSRegistry& registry = getECSRegistry();
    Renderer* renderer = getRenderer();

    cat_img = createSprite(
        registry,
        renderer,
        "baguette.jpg",
        { .pos = Vec2(400.0f, 400.0f),
          .scale = Vec2(0.5f, 0.5f),
          .rotation = 45.0f }
    );
    registry.addComponents(
        cat_img,
        UILayoutComponent{ .anchor = Anchor::CENTER, .pivot = Pivot::CENTER }
    );

    batch = std::make_unique<DynamicBatch>(renderer, nullptr);
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
    UILayoutSystem::update(getECSRegistry(), getWindow()->getSize());
}

void
App::onRender(float alpha)
{
    Game::onRender(alpha);
}
