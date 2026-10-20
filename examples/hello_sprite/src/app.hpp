#pragma once

#include <lili2d/lili2d.hpp>
#include <memory>

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
    std::unique_ptr<lili::DynamicBatch> batch;

    void
    onInit() override;
    void
    onEvent(const lili::Event& event) override;
    void
    onUpdate(float dt) override;
    void
    onRender(float alpha) override;
};
