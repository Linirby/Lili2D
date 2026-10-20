#include "app.hpp"

using namespace lili;

App::App()
  : Game("hello_shapes - Lili2D", 500, 350)
{
    Renderer* renderer = getRenderer();

    thin_line = Line(
        renderer,
        LineShape({ 50.0f, 50.0f }, { 100.0f, 300.0f }, 1.0f),
        Vec4(0.0f, 1.0f, 0.0f, 1.0f)
    );
    large_line = Line(
        renderer,
        LineShape({ 100.0f, 50.0f }, { 150.0f, 300.0f }, 5.0f),
        Vec4(0.0f, 1.0f, 0.0f, 1.0f)
    );

    rect = Rect(
        renderer,
        RectShape(250.0f, 200.0f, 100.0f, 50.0f),
        Vec4(1.0f, 0.0f, 0.0f, 1.0f)
    );

    clean_circle = Circle(
        renderer,
        CircleShape({ 225.0f, 100.0f }, 50.0f, 32),
        Vec4(0.0f, 0.0f, 1.0f, 1.0f)
    );
    low_poly_circle = Circle(
        renderer,
        CircleShape({ 375.0f, 100.0f }, 50.0f, 8),
        Vec4(0.0f, 0.0f, 1.0f, 1.0f)
    );

    rect.setHollowThickness(2.0f);
    clean_circle.setHollowThickness(2.0f);
    low_poly_circle.setHollowThickness(2.0f);
}

void
App::onEvent(const Event& event)
{
    Game::onEvent(event);
    if (event.type() == EventType::KEYBOARD) {
        KeyboardEvent kb = event.keyboard();
        if (kb.action == KeyAction::PRESSED && !kb.repeat) {
            if (kb.key == Key::ESCAPE) {
                shutdown();
            } else if (kb.key == Key::H || kb.key == Key::SPACE) {
                bool hollow = !rect.isHollow();
                rect.setHollow(hollow);
                clean_circle.setHollow(hollow);
                low_poly_circle.setHollow(hollow);
            }
        }
    }
}

void
App::onRender(float alpha)
{
    (void)alpha;
    thin_line.draw();
    large_line.draw();

    rect.draw();

    clean_circle.draw();
    low_poly_circle.draw();
}
