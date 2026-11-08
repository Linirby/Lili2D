#pragma once

#include <lili2d/core/game.hpp>

class App : public lili::Game
{
public:
    App();

private:
    int red_square_layer = 1;
    lili::Entity red_square;

    lili::Entity layer_1;
    lili::Entity layer_2;
    lili::Entity layer_3;

    lili::Entity text_current_layer;
    lili::Entity text_control_info;

    void
    onEvent(const lili::Event& event) override;
    void
    onUpdate(float dt) override;
};
