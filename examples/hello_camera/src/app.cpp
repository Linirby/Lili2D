#include "app.hpp"
#include "lili2d/core/game.hpp"
#include "lili2d/ecs/ecs_registry.hpp"
#include "lili2d/entities/rect.hpp"
#include "lili2d/geometry/vec2.hpp"
#include "lili2d/geometry/vec4.hpp"

App::App()
  : lili::Game("hello_camera - Lili2D", 600, 400)
{
    setTps(20.0f);
    lili::Renderer* renderer = getRenderer();
    getWindow()->setResizable(true);

    camera = lili::Camera();
    cam_pos = { 250.0f, 250.0f };
    camera.setPosition(cam_pos);
    camera_zoom = camera.getZoom();
    renderer->setCamera(&camera);

    camera_center = lili::Circle(
        renderer,
        lili::CircleShape(lili::Vec2(0.0f, 0.0f), 5.0f, 16),
        lili::Vec4(1.0f, 1.0f, 1.0f, 0.5f)
    );
    camera_center.setRender(lili::RenderLayer::UI);
    camera_center.setAnchor(lili::Anchor::CENTER);
    camera_center.setPivot(lili::Pivot::CENTER);

    lili::ECSRegistry& registry = getECSRegistry();
    red_rect = lili::createRect(
        registry,
        lili::Vec2(250.0f, 200.0f),        // pos
        lili::Vec2(100.0f, 50.0f),         // size
        lili::Vec2(1.0f, 1.0f),            // scale
        0.0f,                              // rotation
        lili::Vec4(1.0f, 0.0f, 0.0f, 1.0f) // color
    );
    green_rect = lili::createRect(
        registry,
        lili::Vec2(100.0f, 100.0f),        // pos
        lili::Vec2(75.0f, 75.0f),          // size
        lili::Vec2(1.0f, 1.0f),            // scale
        0.0f,                              // rotation
        lili::Vec4(0.0f, 1.0f, 0.0f, 1.0f) // color
    );
    blue_rect = lili::createRect(
        registry,
        lili::Vec2(400.0f, 250.0f),        // pos
        lili::Vec2(50.0f, 200.0f),         // size
        lili::Vec2(1.0f, 1.0f),            // scale
        0.0f,                              // rotation
        lili::Vec4(0.0f, 0.0f, 1.0f, 1.0f) // color
    );

    lili::BitmapFont* font =
        lili::Assets::loadFont("lili_font", renderer, "lili_font.png", 16, 6);
    text_cam_pos = lili::Text(renderer, font, "");
    text_cam_pos.setScale(2.0f);
    text_cam_pos.setOffset({ 0.0f, 10.0f });
    text_cam_pos.setRender(lili::RenderLayer::UI);
    text_cam_pos.setAnchor(lili::Anchor::TOP);
    text_cam_pos.setPivot(lili::Pivot::TOP);

    text_controls = lili::Text(
        renderer, font, "IJKL/WASD = move the camera | ZX/LMB RMB = zoom/dezoom"
    );
    text_controls.setScale(1.6f);
    text_controls.setOffset({ 0.0f, 32.0f });
    text_controls.setRender(lili::RenderLayer::UI);
    text_controls.setAnchor(lili::Anchor::TOP);
    text_controls.setPivot(lili::Pivot::TOP);

    lili::ActionMap& action_map = lili::ActionMap::get();
    action_map.add("Up", { lili::Key::I, lili::Key::W, lili::Key::UP });
    action_map.add("Down", { lili::Key::K, lili::Key::S, lili::Key::DOWN });
    action_map.add("Left", { lili::Key::J, lili::Key::A, lili::Key::LEFT });
    action_map.add("Right", { lili::Key::L, lili::Key::D, lili::Key::RIGHT });
    action_map.add("Zoom", { lili::Key::Z }, { lili::MouseButton::LEFT });
    action_map.add("Dezoom", { lili::Key::X }, { lili::MouseButton::RIGHT });
}

void
App::onEvent(const lili::Event& event)
{
    lili::Game::onEvent(event);

    if (event.type() == lili::EventType::KEYBOARD) {
        lili::KeyboardEvent kb = event.keyboard();
        if (kb.action == lili::KeyAction::PRESSED &&
            kb.key == lili::Key::ESCAPE)
            shutdown();
    }
}

void
App::onUpdate(float dt)
{
    lili::Game::onUpdate(dt);

    lili::ActionMap& action_map = lili::ActionMap::get();
    action_map.update();

    lili::Vec2 dir{};
    if (action_map.isHeld("Up"))
        dir.y -= 1.0f;
    if (action_map.isHeld("Down"))
        dir.y += 1.0f;
    if (action_map.isHeld("Left"))
        dir.x -= 1.0f;
    if (action_map.isHeld("Right"))
        dir.x += 1.0f;

    cam_pos = camera.getPosition() + dir * (100.0f * dt);
    camera.setPosition(cam_pos);

    if (action_map.isHeld("Zoom"))
        camera_zoom += 2.0f * dt;
    if (action_map.isHeld("Dezoom"))
        camera_zoom -= 2.0f * dt;
    camera.setZoom(camera_zoom);

    text_cam_pos.setText(
        "Camera Position: (X=" + std::to_string(static_cast<int>(cam_pos.x)) +
        ", Y=" + std::to_string(static_cast<int>(cam_pos.y)) + ")"
    );
}

void
App::onRender(float alpha)
{
    lili::Game::onRender(alpha);

    camera_center.draw();
    text_cam_pos.draw();
    text_controls.draw();
}
