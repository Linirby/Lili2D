#pragma once

#include "lili2d/ecs/ecs_registry.hpp"
#include "lili2d/ecs/entity.hpp"
#include "lili2d/geometry/vec2.hpp"
#include "lili2d/geometry/vec4.hpp"
#include "lili2d/render/2d/dynamic_batch.hpp"
#include "lili2d/render/2d/text.hpp"
#include <lili2d/core/event.hpp>
#include <lili2d/core/game.hpp>
#include <vector>

class App : public lili::Game
{
public:
    App()
      : lili::Game("hello_render2d - Lili2D", 1000, 800)
    {
    }

private:
    std::unique_ptr<lili::DynamicBatch> batch;
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
        bool hollow
    );
    void
    addCircle(
        lili::ECSRegistry& registry,
        lili::Vec2 pos,
        float radius,
        lili::Vec4 color,
        bool hollow
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
