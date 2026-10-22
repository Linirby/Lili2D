#pragma once

#include <lili2d/core/game.hpp>
#include <lili2d/ecs/entity.hpp>
#include <lili2d/render/2d/text.hpp>
#include <memory>

struct RectUB
{
    float time;
    float amplitude;
    float frequency;
    float speed;
};

struct TextUB
{
    float speed;
    float time;
};

class App : public lili::Game
{
public:
    App();

private:
    lili::Clock clock;

    std::unique_ptr<lili::MainGraphicsPipeline> rect_pipeline;
    lili::Entity rect;

    std::unique_ptr<lili::MainGraphicsPipeline> text_pipeline;
    lili::Text text;
    lili::Text text_info;

    bool use_custom_shaders;

    void
    toggleCustomShaders();

    void
    onEvent(const lili::Event& event) override;
    void
    onUpdate(float dt) override;
    void
    onRender(float alpha) override;
};
