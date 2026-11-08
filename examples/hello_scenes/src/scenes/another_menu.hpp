#pragma once

#include <lili2d/core/scene_manager.hpp>

class AnotherScene : public lili::Scene
{
public:
    explicit AnotherScene();

    void
    handleEvents(const lili::Event& event) override;

private:
    lili::Entity text;
    lili::Entity info_text;
};
