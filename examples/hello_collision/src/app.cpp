#include "app.hpp"

using namespace lili;

App::App()
  : Game("hello_collision - Lili2D", 800, 800)
{
    Renderer* renderer = getRenderer();
    cursor_rect = Rect(
        renderer,
        RectShape(0.0f, 0.0f, 75.0f, 75.0f),
        Vec4(0.0f, 0.0f, 1.0f, 1.0f)
    );
    cursor_circle = Circle(
        renderer,
        CircleShape(Vec2(0.0f, 0.0f), 30.0f, 16),
        Vec4(0.0f, 0.0f, 1.0f, 1.0f)
    );
    cursor_line = Line(
        renderer,
        LineShape(Vec2(0.0f, 0.0f), Vec2(0.0f, 0.0f), 3.0f),
        Vec4(0.0f, 0.0f, 1.0f, 1.0f)
    );
    random_rect = Rect(
        renderer,
        RectShape(350.0f, 350.0f, 100.0f, 100.0f),
        Vec4(1.0f, 0.0f, 0.0f, 1.0f)
    );
}

void
App::onEvent(const Event& event)
{
    Game::onEvent(event);

    if (event.type() == EventType::KEYBOARD) {
        KeyboardEvent kb = event.keyboard();
        if (kb.action == KeyAction::PRESSED) {
            if (kb.key == Key::ESCAPE)
                shutdown();
            else if (kb.key == Key::NUM_1)
                active_shape = ShapeMode::Rect;
            else if (kb.key == Key::NUM_2)
                active_shape = ShapeMode::Circle;
            else if (kb.key == Key::NUM_3)
                active_shape = ShapeMode::Line;
        }
    }
}

void
App::onUpdate([[maybe_unused]] float dt)
{
    Game::onUpdate(dt);

    Mouse mouse;
    mouse.update();

    const Vec4 hit_color(0.0f, 1.0f, 0.0f, 1.0f);
    const Vec4 miss_color(0.0f, 0.0f, 1.0f, 1.0f);

    switch (active_shape) {
        case ShapeMode::Rect: {
            cursor_rect.setPosition(
                mouse.getPos() - cursor_rect.getSize() * 0.5f
            );
            bool hit =
                AABB2(cursor_rect.getShape()).intersect(random_rect.getShape());
            cursor_rect.setColor(hit ? hit_color : miss_color);
            break;
        }
        case ShapeMode::Circle: {
            cursor_circle.setCenter(mouse.getPos());
            bool hit = CircleCollider(cursor_circle.getShape())
                           .intersect(random_rect.getShape());
            cursor_circle.setColor(hit ? hit_color : miss_color);
            break;
        }
        case ShapeMode::Line: {
            cursor_line.setEnd(
                mouse.getPos() - Vec2(cursor_line.getThickness(), 0.0f) * 0.5f
            );
            bool hit =
                AABB2(cursor_line.getShape()).intersect(random_rect.getShape());
            cursor_line.setColor(hit ? hit_color : miss_color);
            break;
        }
    }
}

void
App::onRender([[maybe_unused]] float alpha)
{
    Game::onRender(alpha);

    random_rect.draw();
    Renderer* renderer = getRenderer();
    const Vec4 debug_color(0.0f, 1.0f, 0.0f, 1.0f);

    switch (active_shape) {
        case ShapeMode::Rect:
            cursor_rect.draw();
            renderer->drawRect(cursor_rect.getShape(), debug_color, true);
            break;
        case ShapeMode::Circle:
            cursor_circle.draw();
            renderer->drawCircle(cursor_circle.getShape(), debug_color, true);
            break;
        case ShapeMode::Line: {
            cursor_line.draw();
            AABB2 line_box(cursor_line.getShape());
            renderer->drawRect(line_box.getShape(), debug_color, true);
            break;
        }
    }
}
