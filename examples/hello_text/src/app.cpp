#include "app.hpp"
#include <lili2d/components/text.hpp>
#include <lili2d/components/transform.hpp>
#include <lili2d/components/ui_layout.hpp>
#include <lili2d/core/game.hpp>
#include <lili2d/ecs/ecs_registry.hpp>
#include <lili2d/entities/text.hpp>
#include <lili2d/geometry/vec3.hpp>
#include <lili2d/geometry/vec4.hpp>
#include <lili2d/render/gpu/pass_types.hpp>
#include <lili2d/render/renderer.hpp>

using namespace lili;

App::App()
  : Game("hello_text - Lili2D", 625, 300)
{
    getWindow()->setResizable(true);
    Renderer* renderer = getRenderer();
    ECSRegistry& registry = getECSRegistry();

    BitmapFont* font_example =
        Assets::loadFont("lili_font", renderer, "lili_font.png", 16, 6);

    letter_spacing = 1.0f;
    text_rotation = 0.0f;
    welcome_text = createText(
        registry,
        font_example,
        TextParameters{
            .text = "Welcome to Lili2d!",
            .scale = Vec2(3.0f, 3.0f),
            .rotation = text_rotation,
            .spacing = letter_spacing,
            .render_pass = lili::RenderLayer::UI,
        }
    );
    registry.addComponents(welcome_text, UILayoutComponent{});

    info_text = Text(
        renderer, font_example, "<- & ->: change letter spacing | R: rotate"
    );
    info_text.setScale(3.0f);
    info_text.setPosition({ 10.0f, 10.0f });
}

void
App::onEvent(const Event& event)
{
    Game::onEvent(event);

    if (event.type() == EventType::KEYBOARD) {
        KeyboardEvent kb = event.keyboard();
        if (kb.action == KeyAction::PRESSED && kb.key == Key::ESCAPE)
            shutdown();
    }
}

void
App::onUpdate(float dt)
{
    Game::onUpdate(dt);
    ECSRegistry& registry = getECSRegistry();

    TextComponent& text_comp =
        registry.getComponent<TextComponent>(welcome_text);
    TransformComponent& trans_comp =
        registry.getComponent<TransformComponent>(welcome_text);

    keyboard.update();
    if (keyboard.held(Key::LEFT)) {
        letter_spacing -= speed_change * dt;
        text_comp.spacing = letter_spacing;
    }
    if (keyboard.held(Key::RIGHT)) {
        letter_spacing += speed_change * dt;
        text_comp.spacing = letter_spacing;
    }
    if (keyboard.held(Key::R)) {
        text_rotation += 5.0f * speed_change * dt;
        trans_comp.rotation = text_rotation;
    }
}

void
App::onRender(float alpha)
{
    Game::onRender(alpha);

    info_text.draw();
}
