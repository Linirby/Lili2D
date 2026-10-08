#pragma once

#include "lili2d/ecs/entity.hpp"
// #include "lili2d/render/2d/sprite.hpp"
#include "lili2d/render/2d/dynamic_batch.hpp"
#include <lili2d/lili2d.hpp>
#include <memory>

class App : public lili::Game
{
public:
    App();

private:
    lili::Entity cat_img;
    std::unique_ptr<lili::DynamicBatch> batch;
    // lili::Sprite cat_sprite;

    void
    onEvent(const lili::Event& event) override;
    void
    onRender(float alpha) override;
};
