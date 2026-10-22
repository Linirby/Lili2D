#include "app.hpp"

using namespace lili;

App::App()
  : Game("hello_text - Lili2D", 625, 300)
{
    getWindow()->setResizable(true);
    Renderer* renderer = getRenderer();

    BitmapFont* font_example =
        Assets::loadFont("lili_font", renderer, "lili_font.png", 16, 6);

    welcome_text = Text(renderer, font_example, "Welcome to Lili2D");
    welcome_text.setScale(3.0f);
    welcome_text.setSpacing(letter_spacing);
    welcome_text.setRender(RenderLayer::UI);
    welcome_text.setAnchor(Anchor::CENTER);
    welcome_text.setPivot(Pivot::CENTER);

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
    keyboard.update();
    if (keyboard.held(Key::LEFT)) {
        letter_spacing -= speed_change * dt;
        welcome_text.setSpacing(letter_spacing);
    }
    if (keyboard.held(Key::RIGHT)) {
        letter_spacing += speed_change * dt;
        welcome_text.setSpacing(letter_spacing);
    }
    if (keyboard.held(Key::R)) {
        text_rotation += 5.0f * speed_change * dt;
        welcome_text.setRotation(text_rotation);
    }
}

void
App::onRender([[maybe_unused]] float alpha)
{
    welcome_text.draw();
    info_text.draw();
}
