#pragma once

#include <lili2d/core/event.hpp>
#include <lili2d/core/scene.hpp>
#include <lili2d/ecs/ecs_registry.hpp>
#include <lili2d/ecs/entity.hpp>
#include <lili2d/render/renderer.hpp>

class MainScene : public lili::Scene
{
public:
    explicit MainScene();

    void
    handleEvents(const lili::Event& event) override;

private:
    lili::Entity text;
    lili::Entity info_text;
};
