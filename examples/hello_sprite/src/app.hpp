#pragma once

#include "lili2d/ecs/entity.hpp"
// #include "lili2d/render/2d/sprite.hpp"
#include "lili2d/render/2d/dynamic_batch.hpp"
#include <lili2d/lili2d.hpp>
#include <memory>

class App : public lili::Game
{
public:
    App()
      : lili::Game("hello_sprite - Lili2D", 800, 800)
    {
    }

private:
    lili::Entity cat_img;
    std::unique_ptr<lili::DynamicBatch> batch;

    void
    onInit() override;
    void
    onEvent(const lili::Event& event) override;
    void
    onRender(float alpha) override;
};
