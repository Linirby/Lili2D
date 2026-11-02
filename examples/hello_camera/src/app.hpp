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

    lili::Entity text_cam_pos;
    lili::Entity text_controls;

    static constexpr size_t MAX_CAM_TEXT_LENGTH = 64;

    void
    onEvent(const lili::Event& event) override;
    void
    onUpdate(float dt) override;
};
