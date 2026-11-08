#pragma once

#include <lili2d/core/game.hpp>

class App : public lili::Game
{
public:
    App();

private:
    void
    onEvent(const lili::Event& event) override;
    void
    onUpdate(float dt) override;
};
