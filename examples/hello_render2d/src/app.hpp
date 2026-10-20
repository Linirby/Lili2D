#pragma once

#include <lili2d/lili2d.hpp>
#include <vector>

class App : public lili::Game
{
public:
    App()
      : lili::Game("hello_render2d - Lili2D", 1000, 800)
    {
        getWindow()->setResizable(true);
    }

private:
    std::vector<lili::Entity> spawned;

    lili::Text instructions_text;

    void
    addBaguette(
        lili::ECSRegistry& registry,
        lili::Renderer* renderer,
        lili::Vec2 pos
    );
    void
    addRect(
        lili::ECSRegistry& registry,
        lili::Vec2 pos,
        lili::Vec2 size,
        lili::Vec4 color,
        bool hollow,
        float thickness
    );
    void
    addCircle(
        lili::ECSRegistry& registry,
        lili::Vec2 pos,
        float radius,
        lili::Vec4 color,
        bool hollow,
        float thickness
    );
    void
    addLine(
        lili::ECSRegistry& registry,
        lili::Vec2 start_pos,
        lili::Vec2 end_pos,
        float thickness,
        lili::Vec4 color
    );
    void
    popLast(lili::ECSRegistry& registry);

    void
    onInit() override;
    void
    onEvent(const lili::Event& event) override;
    void
    onRender(float alpha) override;
};
