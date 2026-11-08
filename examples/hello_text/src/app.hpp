#pragma once

#include <lili2d/core/game.hpp>
#include <lili2d/core/keyboard.hpp>
#include <lili2d/ecs/entity.hpp>

class App : public lili::Game
{
public:
    App();

private:
    float letter_spacing = 1.0f;
    float text_rotation = 0.0f;
    float speed_change = 10.0f;

    lili::Entity welcome_text;
    lili::Entity info_text;
    lili::Keyboard keyboard;

    void
    onEvent(const lili::Event& event) override;
    void
    onUpdate(float dt) override;
};
