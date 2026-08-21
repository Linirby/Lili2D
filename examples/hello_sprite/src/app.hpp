#pragma once

#include <lili2d/core.hpp>
#include <lili2d/render.hpp>

class App : public lili::Game {
public:
    App();

private:
    lili::Sprite cat_sprite;

    void
    onEvent(const lili::Event& event) override;
    void
    onRender(float alpha) override;
};
