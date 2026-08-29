#pragma once

#include <lili2d/lili2d.hpp>

class App : public lili::Game {
public:
    App();

private:
    enum class ShapeMode { Rect, Circle, Line };
    ShapeMode active_shape = ShapeMode::Rect;

    lili::Rect cursor_rect;
    lili::Circle cursor_circle;
    lili::Line cursor_line;
    lili::Rect random_rect;

    void
    onEvent(const lili::Event& event) override;
    void
    onUpdate(float dt) override;
    void
    onRender(float alpha) override;
};
