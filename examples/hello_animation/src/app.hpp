#pragma once

#include <lili2d/core/game.hpp>
#include <lili2d/render/2d/animated_sprite.hpp>

class App : public lili::Game
{
public:
    App();

private:
    lili::AnimatedSprite head_sprite;

    void
    onEvent(const lili::Event& event) override;
    void
    onUpdate(float dt) override;
    void
    onRender(float alpha) override;
};
