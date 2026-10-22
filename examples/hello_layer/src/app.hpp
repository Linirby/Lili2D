#pragma once

#include <lili2d/ecs/entity.hpp>
#include <lili2d/lili2d.hpp>

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

    lili::Text text_current_layer;
    lili::Text text_control_info;

    void
    onEvent(const lili::Event& event) override;
    void
    onRender(float alpha) override;
};
