#pragma once

#include <lili2d/ecs/entity.hpp>
#include <lili2d/lili2d.hpp>

class App : public lili::Game
{
public:
    App();

private:
    lili::Entity thin_line;
    lili::Entity large_line;
    lili::Entity rect;
    lili::Entity clean_circle;
    lili::Entity low_poly_circle;

    bool hollow_active;

    void
    toggleHollow();

    void
    onEvent(const lili::Event& event) override;
};
