#include "app.hpp"

#include <format>
#include <lili2d/components/circle.hpp>
#include <lili2d/components/render2d.hpp>
#include <lili2d/components/transform.hpp>
#include <lili2d/core/action_map.hpp>
#include <lili2d/core/string_arena.hpp>
#include <lili2d/entities/circle.hpp>
#include <lili2d/entities/rect.hpp>
#include <lili2d/entities/text.hpp>
#include <lili2d/geometry/vec2.hpp>
#include <lili2d/geometry/vec4.hpp>
#include <lili2d/render/gpu/pass_types.hpp>
#include <string>

using namespace lili;

App::App()
  : Game("hello_camera - Lili2D", 600, 400)
{
    setTps(20.0f);
    getWindow()->setResizable(true);

    Renderer* renderer = getRenderer();
    ECSRegistry& registry = getECSRegistry();

    cam_pos = { 250.0f, 250.0f };
    camera_zoom = camera.getZoom();
    camera.setPosition(cam_pos);
    renderer->setCamera(&camera);

    camera_center = createCircle(
        registry,
        {
            .transform{
                .pos = Vec2(0.0f, 0.0f),
            },
            .render2d{
                .color = Vec4(1.0f, 1.0f, 1.0f, 0.5f),
                .render_pass = lili::RenderLayer::UI,
            },
            .circle{
                .radius = 5.0f,
            },
        }
    );
    registry.addComponents(
        camera_center,
        UILayoutComponent{ .anchor = Anchor::CENTER, .pivot = Pivot::CENTER }
    );

    red_rect = createRect(
        registry,
        {
            .transform{
                .pos = Vec2(250.0f, 200.0f),
            },
            .render2d{
                .color = Vec4(1.0f, 0.0f, 0.0f, 1.0f),
                .size = Vec2(100.0f, 50.0f),
            },
        }
    );
    green_rect = createRect(
        registry,
        {
            .transform{
                .pos = Vec2(100.0f, 100.0f),
            },
            .render2d{
                .color = Vec4(0.0f, 1.0f, 0.0f, 1.0f),
                .size = Vec2(75.0f, 75.0f),
            },
        }
    );
    blue_rect = createRect(
        registry,
        {
            .transform{
                .pos = Vec2(400.0f, 250.0f),
            },
            .render2d{
                .color = Vec4(0.0f, 0.0f, 1.0f, 1.0f),
                .size = Vec2(50.0f, 200.0f),
            },
        }
    );

    text_cam_pos = createText(
        registry,
        std::string(MAX_CAM_TEXT_LENGTH, ' '),
        {
            .transform{ .scale = Vec2(2.0f, 2.0f) },
            .render2d{ .render_pass = lili::RenderLayer::UI },
        }
    );
    registry.addComponents(
        text_cam_pos,
        UILayoutComponent{
            .offset = Vec2(0.0f, 10.0f),
            .anchor = lili::Anchor::TOP,
            .pivot = lili::Pivot::TOP,
        }
    );

    text_controls = createText(
        registry,
        "IJKL/WASD = move the camera | ZX/LMB RMB = zoom/dezoom",
        {
            .transform{ .scale = Vec2(1.6f, 1.6f) },
            .render2d{ .render_pass = lili::RenderLayer::UI },
        }
    );
    registry.addComponents(
        text_controls,
        UILayoutComponent{
            .offset = Vec2(0.0f, 32.0f),
            .anchor = lili::Anchor::TOP,
            .pivot = lili::Pivot::TOP,
        }
    );

    ActionMap& action_map = ActionMap::get();
    action_map.add("Up", { Key::I, Key::W, Key::UP });
    action_map.add("Down", { Key::K, Key::S, Key::DOWN });
    action_map.add("Left", { Key::J, Key::A, Key::LEFT });
    action_map.add("Right", { Key::L, Key::D, Key::RIGHT });
    action_map.add("Zoom", { Key::Z }, { MouseButton::LEFT });
    action_map.add("Dezoom", { Key::X }, { MouseButton::RIGHT });
}

void
App::onEvent(const Event& event)
{
    Game::onEvent(event);

    if (event.type() == EventType::KEYBOARD) {
        KeyboardEvent kb = event.keyboard();
        if (kb.action == KeyAction::PRESSED && kb.key == Key::ESCAPE)
            shutdown();
    }
}

void
App::onUpdate(float dt)
{
    Game::onUpdate(dt);

    ActionMap& action_map = ActionMap::get();
    Vec2 dir{};
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

    ECSRegistry& ecs_reg = getECSRegistry();
    auto& text_comp = ecs_reg.getComponent<TextComponent>(text_cam_pos);

    text_comp.length = ecs_reg.getStringArena().formatAt(
        text_comp.offset,
        MAX_CAM_TEXT_LENGTH,
        "Camera Position: (X={:.1f}, Y={:.1f})",
        cam_pos.x,
        cam_pos.y
    );
}
