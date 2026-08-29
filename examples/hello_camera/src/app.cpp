#include "app.hpp"

App::App() : lili::Game("hello_camera - Lili2D", 600, 400) {
    setTps(20.0f);
    lili::Renderer* renderer = getRenderer();
    getWindow()->setResizable(true);

    camera = lili::Camera();
    cam_pos = {250.0f, 250.0f};
    camera.setPosition(cam_pos);
    camera_zoom = camera.getZoom();
    renderer->setCamera(&camera);

    camera_center = lili::Circle(
        renderer, lili::CircleShape(lili::Vec2(0.0f, 0.0f), 5.0f, 16),
        lili::Vec4(1.0f, 1.0f, 1.0f, 0.5f)
    );
    camera_center.setRender(lili::RenderLayer::UI);
    camera_center.setAnchor(lili::Anchor::CENTER);
    camera_center.setPivot(lili::Pivot::CENTER);

    red_rect = lili::Rect(
        renderer, lili::RectShape(250.0f, 200.0f, 100.0f, 50.0f),
        lili::Vec4(1.0f, 0.0f, 0.0f, 1.0f)
    );
    green_rect = lili::Rect(
        renderer, lili::RectShape(100.0f, 100.0f, 75.0f, 75.0f),
        lili::Vec4(0.0f, 1.0f, 0.0f, 1.0f)
    );
    blue_rect = lili::Rect(
        renderer, lili::RectShape(400.0f, 250.0f, 50.0f, 200.0f),
        lili::Vec4(0.0f, 0.0f, 1.0f, 1.0f)
    );

    lili::BitmapFont* font =
        lili::Assets::loadFont("lili_font", renderer, "lili_font.png", 16, 6);
    text_cam_pos = lili::Text(renderer, font, "");
    text_cam_pos.setScale(2.0f);
    text_cam_pos.setOffset({0.0f, 10.0f});
    text_cam_pos.setRender(lili::RenderLayer::UI);
    text_cam_pos.setAnchor(lili::Anchor::TOP);
    text_cam_pos.setPivot(lili::Pivot::TOP);

    text_controls =
        lili::Text(renderer, font, "IJKL = move the camera | ZX = zoom/dezoom");
    text_controls.setScale(2.0f);
    text_controls.setOffset({0.0f, 32.0f});
    text_controls.setRender(lili::RenderLayer::UI);
    text_controls.setAnchor(lili::Anchor::TOP);
    text_controls.setPivot(lili::Pivot::TOP);

    lili::ActionMap& action_map = lili::ActionMap::get();
    action_map.add("MoveUp", {lili::Key::I, lili::Key::W, lili::Key::UP});
    action_map.add("MoveDown", {lili::Key::K, lili::Key::S, lili::Key::DOWN});
    action_map.add("MoveLeft", {lili::Key::J, lili::Key::A, lili::Key::LEFT});
    action_map.add("MoveRight", {lili::Key::L, lili::Key::D, lili::Key::RIGHT});
    action_map.add("Zoom", {lili::Key::Z}, {lili::MouseButton::LEFT});
    action_map.add("Dezoom", {lili::Key::X}, {lili::MouseButton::RIGHT});
}

void
App::onEvent(const lili::Event& event) {
    lili::Game::onEvent(event);
    if (event.type() == lili::EventType::KEYBOARD) {
        lili::KeyboardEvent kb = event.keyboard();
        if (kb.action == lili::KeyAction::PRESSED && kb.key == SDLK_ESCAPE)
            shutdown();
    }
}

void
App::onUpdate(float dt) {
    lili::ActionMap& action_map = lili::ActionMap::get();
    action_map.update();

    lili::Vec2 dir{};
    if (action_map.isHeld("MoveUp")) dir.y -= 1.0f;
    if (action_map.isHeld("MoveDown")) dir.y += 1.0f;
    if (action_map.isHeld("MoveLeft")) dir.x -= 1.0f;
    if (action_map.isHeld("MoveRight")) dir.x += 1.0f;

    cam_pos = camera.getPosition() + dir * (100.0f * dt);
    camera.setPosition(cam_pos);

    if (action_map.isHeld("Zoom")) camera_zoom += 2.0f * dt;
    if (action_map.isHeld("Dezoom")) camera_zoom -= 2.0f * dt;
    camera.setZoom(camera_zoom);

    text_cam_pos.setText(
        "Camera Position: (X=" + std::to_string(static_cast<int>(cam_pos.x)) +
        ", Y=" + std::to_string(static_cast<int>(cam_pos.y)) + ")"
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
