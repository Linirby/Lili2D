#include "app.hpp"

#include "lili2d/core/action_map.hpp"
#include "lili2d/core/event.hpp"
#include "lili2d/core/keys.hpp"
#include "lili2d/render/ui/ui_layout.hpp"

App::App() : lili::Game("hello_camera - Lili2D", 600, 400) {
    setTps(20.0f);
    lili::Window* window = getWindow();
    lili::Renderer* renderer = getRenderer();

    window->setResizable(true);

    camera = lili::Camera();
    cam_pos = {250, 250};
    camera.setPosition(cam_pos);
    camera_zoom = camera.getZoom();
    camera_center = lili::Circle(
        renderer, lili::CircleShape(lili::Vec2(0.0f, 0.0f), 5, 16),
        lili::Vec4(1, 1, 1, 0.5)
    );
    camera_center.setRender(lili::RenderLayer::UI);
    camera_center.setAnchor(lili::Anchor::Center);
    camera_center.setPivot(lili::Pivot::Center);
    renderer->setCamera(&camera);

    red_rect = lili::Rect(
        renderer, lili::RectShape(250, 200, 100, 50), lili::Vec4(1, 0, 0, 1)
    );
    green_rect = lili::Rect(
        renderer, lili::RectShape(100, 100, 75, 75), lili::Vec4(0, 1, 0, 1)
    );
    blue_rect = lili::Rect(
        renderer, lili::RectShape(400, 250, 50, 200), lili::Vec4(0, 0, 1, 1)
    );

    lili::BitmapFont* font =
        lili::Assets::loadFont("lili_font", renderer, "lili_font.png", 16, 6);
    text_cam_pos = lili::Text(renderer, font, "");
    text_cam_pos.setScale(2);
    text_cam_pos.setPosition({0, 10});
    text_cam_pos.setRender(lili::RenderLayer::UI);
    text_cam_pos.setAnchor(lili::Anchor::Top);
    text_cam_pos.setPivot(lili::Pivot::Top);
    text_controls =
        lili::Text(renderer, font, "IJKL = move the camera | ZX = zoom/dezoom");
    text_controls.setScale(2);
    text_controls.setPosition({0, 32});
    text_controls.setRender(lili::RenderLayer::UI);
    text_controls.setAnchor(lili::Anchor::Top);
    text_controls.setPivot(lili::Pivot::Top);

    lili::ActionMap& action_map = lili::ActionMap::get();
    action_map.add("MoveUp", {lili::Key::I, lili::Key::W, lili::Key::Up});
    action_map.add("MoveDown", {lili::Key::K, lili::Key::S, lili::Key::Down});
    action_map.add("MoveLeft", {lili::Key::J, lili::Key::A, lili::Key::Left});
    action_map.add("MoveRight", {lili::Key::L, lili::Key::D, lili::Key::Right});
    action_map.add("Zoom", {lili::Key::Z}, {lili::MouseButton::LEFT});
    action_map.add("Dezoom", {lili::Key::X}, {lili::MouseButton::RIGHT});
}

void
App::onEvent(const lili::Event& event) {
    lili::KeyboardEvent kb = event.keyboard();

    if (event.type() == lili::EventType::KEYBOARD)
        if (kb.action == lili::KeyAction::PRESSED)
            if (kb.key == SDLK_ESCAPE) shutdown();
}

void
App::onUpdate(float dt) {
    lili::ActionMap& action_map = lili::ActionMap::get();
    action_map.update();
    cam_pos = camera.getPosition();
    if (action_map.isHeld("MoveUp")) cam_pos.y -= 100.0f * dt;
    if (action_map.isHeld("MoveDown")) cam_pos.y += 100.0f * dt;
    if (action_map.isHeld("MoveLeft")) cam_pos.x -= 100.0f * dt;
    if (action_map.isHeld("MoveRight")) cam_pos.x += 100.0f * dt;
    camera.setPosition(cam_pos);

    if (action_map.isHeld("Zoom")) camera_zoom += 2.0f * dt;
    if (action_map.isHeld("Dezoom")) camera_zoom -= 2.0f * dt;
    camera.setZoom(camera_zoom);

    text_cam_pos.setText(
        "Camera Position: (X=" + std::to_string(cam_pos.x) +
        ", Y=" + std::to_string(cam_pos.y) + ")"
    );
}

void
App::onRender(float alpha) {
    (void)alpha;
    camera_center.draw();

    red_rect.draw();
    green_rect.draw();
    blue_rect.draw();

    text_cam_pos.draw();
    text_controls.draw();
}
