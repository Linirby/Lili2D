#include "app.hpp"

App::App() : lili::Game("hello_collision - Lili2D", 800, 800) {
    lili::Renderer* renderer = getRenderer();
    cursor_rect = lili::Rect(
        renderer, lili::RectShape(0.0f, 0.0f, 75.0f, 75.0f),
        lili::Vec4(0.0f, 0.0f, 1.0f, 1.0f)
    );
    cursor_circle = lili::Circle(
        renderer, lili::CircleShape(lili::Vec2(0.0f, 0.0f), 30.0f, 16),
        lili::Vec4(0.0f, 0.0f, 1.0f, 1.0f)
    );
    cursor_line = lili::Line(
        renderer,
        lili::LineShape(lili::Vec2(0.0f, 0.0f), lili::Vec2(0.0f, 0.0f), 3.0f),
        lili::Vec4(0.0f, 0.0f, 1.0f, 1.0f)
    );
    random_rect = lili::Rect(
        renderer, lili::RectShape(350.0f, 350.0f, 100.0f, 100.0f),
        lili::Vec4(1.0f, 0.0f, 0.0f, 1.0f)
    );
}

void
App::onEvent(const lili::Event& event) {
    lili::Game::onEvent(event);
    if (event.type() == lili::EventType::KEYBOARD) {
        lili::KeyboardEvent kb = event.keyboard();
        if (kb.action == lili::KeyAction::PRESSED) {
            if (kb.key == lili::Key::ESCAPE) shutdown();
            else if (kb.key == lili::Key::NUM_1) active_shape = ShapeMode::Rect;
            else if (kb.key == lili::Key::NUM_2) active_shape = ShapeMode::Circle;
            else if (kb.key == lili::Key::NUM_3) active_shape = ShapeMode::Line;
        }
    }
}

void
App::onUpdate(float dt) {
    (void)dt;
    lili::Mouse mouse;
    mouse.update();

    const lili::Vec4 hit_color(0.0f, 1.0f, 0.0f, 1.0f);
    const lili::Vec4 miss_color(0.0f, 0.0f, 1.0f, 1.0f);

    switch (active_shape) {
        case ShapeMode::Rect: {
            cursor_rect.setPosition(
                mouse.getPos() - cursor_rect.getSize() * 0.5f
            );
            bool hit = lili::AABB2(cursor_rect.getShape())
                           .intersect(random_rect.getShape());
            cursor_rect.setColor(hit ? hit_color : miss_color);
            break;
        }
        case ShapeMode::Circle: {
            cursor_circle.setCenter(mouse.getPos());
            bool hit = lili::CircleCollider(cursor_circle.getShape())
                           .intersect(random_rect.getShape());
            cursor_circle.setColor(hit ? hit_color : miss_color);
            break;
        }
        case ShapeMode::Line: {
            cursor_line.setEnd(
                mouse.getPos() -
                lili::Vec2(cursor_line.getThickness(), 0.0f) * 0.5f
            );
            bool hit = lili::AABB2(cursor_line.getShape())
                           .intersect(random_rect.getShape());
            cursor_line.setColor(hit ? hit_color : miss_color);
            break;
        }
    }
}

void
App::onRender(float alpha) {
    (void)alpha;
    random_rect.draw();
    lili::Renderer* renderer = getRenderer();
    const lili::Vec4 debug_color(0.0f, 1.0f, 0.0f, 1.0f);

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
            lili::AABB2 line_box(cursor_line.getShape());
            renderer->drawRect(line_box.getShape(), debug_color, true);
            break;
        }
    }
}
