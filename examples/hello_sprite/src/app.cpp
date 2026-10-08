#include "app.hpp"
#include "lili2d/ecs/ecs_registry.hpp"
#include "lili2d/entities/sprite.hpp"
#include "lili2d/geometry/vec2.hpp"
#include "lili2d/render/2d/render2d_system.hpp"
#include "lili2d/render/2d/sprite.hpp"
#include "lili2d/render/renderer.hpp"
#include <memory>

App::App()
  : lili::Game("hello_sprite - Lili2D", 800, 800)
{
    lili::ECSRegistry& ecs_registry = getECSRegistry();
    lili::Renderer* renderer = getRenderer();

    lili::Vec2 pos = { 400.0f, 400.0f };
    lili::Vec2 scale = { 0.5f, 0.5f };
    float rotation = 45.0f;
    cat_img = lili::createSprite(
        ecs_registry, renderer, "cat.png", pos, scale, rotation
    );

    lili::SpriteComponent& sprite =
        ecs_registry.getComponent<lili::SpriteComponent>(cat_img);
    batch = std::make_unique<lili::DynamicBatch>(renderer, sprite.slice.texture);

    // lili::Texture* cat_tex = lili::Assets::loadTexture(
    //     "cat_texture", "cat.png", getRenderer()->getDevice()
    // );
    // cat_sprite = lili::Sprite(getRenderer(), cat_tex);
    // cat_sprite.setScale({ 0.5f, 0.5f });
    // cat_sprite.setPosition({ 400.0f, 50.0f });
    // cat_sprite.setRotation(45.0f);
}

void
App::onEvent(const lili::Event& event)
{
    lili::Game::onEvent(event);
    if (event.type() == lili::EventType::KEYBOARD) {
        lili::KeyboardEvent kb = event.keyboard();
        if (kb.action == lili::KeyAction::PRESSED &&
            kb.key == lili::Key::ESCAPE)
            shutdown();
    }
}

void
App::onRender(float alpha)
{
    (void)alpha;
    lili::Render2DSystem::render(getECSRegistry(), *batch);
    // cat_sprite.draw();
}
