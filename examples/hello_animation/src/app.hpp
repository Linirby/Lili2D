#pragma once

#include <lili2d/core/game.hpp>
#include <lili2d/ecs/entity.hpp>
#include <lili2d/render/2d/animated_sprite.hpp>

class App : public lili::Game
{
public:
    App();

private:
    lili::Entity head_sprite;

    void
    onInit() override;
    void
    onEvent(const lili::Event& event) override;
};
