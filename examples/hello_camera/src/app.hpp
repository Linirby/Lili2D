#pragma once

#include <lili2d/core/game.hpp>
#include <lili2d/render/2d/text.hpp>

class App : public lili::Game
{
public:
    App();

private:
    lili::Camera camera;
    lili::Entity camera_center;
    lili::Vec2 cam_pos;
    float camera_zoom;

    lili::Entity red_rect;
    lili::Entity green_rect;
    lili::Entity blue_rect;

    lili::Text text_cam_pos;
    lili::Text text_controls;

    void
    onEvent(const lili::Event& event) override;
    void
    onUpdate(float dt) override;
    void
    onRender(float alpha) override;
};
