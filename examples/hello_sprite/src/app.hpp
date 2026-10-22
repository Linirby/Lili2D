#pragma once

#include <lili2d/lili2d.hpp>

class App : public lili::Game
{
public:
    App()
      : lili::Game("hello_sprite - Lili2D", 800, 800)
    {
        getWindow()->setResizable(true);
    }

private:
    lili::Entity cat_img;

    void
    onInit() override;
    void
    onEvent(const lili::Event& event) override;
};
