#pragma once

#include "cursor.hpp"
#include <lili2d/core/game.hpp>

class App : public lili::Game
{
public:
    App();

private:
    ShapeMode active_shape = ShapeMode::RECT;

    lili::Entity cursor_rect;
    lili::Entity cursor_circle;
    lili::Entity cursor_line;
    lili::Entity random_rect;
    lili::Entity line_debug_box;

    void
    onEvent(const lili::Event& event) override;
    void
    onUpdate(float dt) override;
};
