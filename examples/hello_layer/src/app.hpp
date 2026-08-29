#pragma once

#include <lili2d/lili2d.hpp>

class App : public lili::Game {
public:
    App();

private:
    lili::Sprite layer_1;
    lili::Sprite layer_2;
    lili::Sprite layer_3;

    lili::Rect red_square;
    int red_square_layer = 1;

    lili::Text text_current_layer;
    lili::Text text_control_info;

    void
    onEvent(const lili::Event& event) override;
    void
    onRender(float alpha) override;
};
