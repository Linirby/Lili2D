#pragma once

#include <lili2d/core/scene.hpp>
#include <lili2d/ecs/ecs_registry.hpp>
#include <lili2d/ecs/entity.hpp>

class YetAgainScene : public lili::Scene
{
public:
    explicit YetAgainScene();

    void
    handleEvents(const lili::Event& event) override;

private:
    lili::Entity text;
    lili::Entity info_text;
};
