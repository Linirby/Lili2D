#include "app.hpp"
#include "lili2d/core/game.hpp"
#include "lili2d/core/window.hpp"
#include "lili2d/entities/circle.hpp"
#include "lili2d/entities/line.hpp"
#include "lili2d/render/2d/text.hpp"
#include "lili2d/render/gpu/pass_types.hpp"
#include "rng_helper.hpp"

#include <lili2d/core/action_map.hpp>
#include <lili2d/core/event.hpp>
#include <lili2d/core/keys.hpp>
#include <lili2d/ecs/ecs_registry.hpp>
#include <lili2d/ecs/entity.hpp>
#include <lili2d/entities/rect.hpp>
#include <lili2d/entities/sprite.hpp>
#include <lili2d/geometry/vec2.hpp>
#include <lili2d/geometry/vec4.hpp>
#include <lili2d/render/2d/render2d_system.hpp>
#include <lili2d/render/renderer.hpp>
#include <vector>

void
App::addBaguette(
    lili::ECSRegistry& registry,
    lili::Renderer* renderer,
    lili::Vec2 pos
)
{
    lili::Entity e = lili::createSprite(
        registry, renderer, "baguette.jpg", pos, { 0.5f, 0.5f }
    );
    spawned.push_back(e);
}

void
App::addRect(
    lili::ECSRegistry& registry,
    lili::Vec2 pos,
    lili::Vec2 size,
    lili::Vec4 color,
    bool hollow,
    float thickness
)
{
    lili::Entity e = lili::createRect(
        registry, pos, size, { 1.0f, 1.0f }, 0.0f, color, hollow, thickness
    );
    spawned.push_back(e);
}

void
App::addCircle(
    lili::ECSRegistry& registry,
    lili::Vec2 pos,
    float radius,
    lili::Vec4 color,
    bool hollow,
    float thickness
)
{
    lili::Entity e =
        lili::createCircle(registry, pos, radius, 24, color, hollow, thickness);
    spawned.push_back(e);
}

void
App::addLine(
    lili::ECSRegistry& registry,
    lili::Vec2 start_pos,
    lili::Vec2 end_pos,
    float thickness,
    lili::Vec4 color
)
{
    lili::Entity e =
        lili::createLine(registry, start_pos, end_pos, thickness, color);
    spawned.push_back(e);
}

void
App::popLast(lili::ECSRegistry& registry)
{
    if (!spawned.empty()) {
        lili::Entity last = spawned.back();
        spawned.pop_back();
        registry.destroyEntity(last);
    }
}

void
App::onInit()
{
    lili::ECSRegistry& registry = getECSRegistry();
    lili::Renderer* renderer = getRenderer();

    instructions_text = lili::Text(
        renderer,
        renderer->getDefaultFont(),
        "Key 1 to 4: Create renderable entity | Backspace: Remove last entity"
    );
    instructions_text.setScale(2.0f);
    instructions_text.setRender(lili::RenderLayer::UI);
    instructions_text.setPosition({ 10.0f, 10.0f });
}

void
App::onEvent(const lili::Event& event)
{
    lili::Game::onEvent(event);

    lili::ECSRegistry& registry = getECSRegistry();
    lili::Renderer* renderer = getRenderer();
    lili::Vec2 window_size = getWindow()->getSize();

    if (event.type() == lili::EventType::KEYBOARD) {
        lili::KeyboardEvent kb_ev = event.keyboard();
        if (event.keyJustPressed(kb_ev)) {
            if (kb_ev.key == lili::Key::ESCAPE)
                shutdown();
            else if (kb_ev.key == lili::Key::NUM_1)
                addBaguette(registry, renderer, rng::pos(window_size));
            else if (kb_ev.key == lili::Key::NUM_2)
                addRect(
                    registry,
                    rng::pos(window_size),
                    rng::size(),
                    rng::color(),
                    rng::hollow(),
                    rng::thickness()
                );
            else if (kb_ev.key == lili::Key::NUM_3)
                addCircle(
                    registry,
                    rng::pos(window_size),
                    rng::radius(),
                    rng::color(),
                    rng::hollow(),
                    rng::thickness()
                );
            else if (kb_ev.key == lili::Key::NUM_4)
                addLine(
                    registry,
                    rng::pos(window_size),
                    rng::pos(window_size),
                    rng::thickness(),
                    rng::color()
                );
            else if (kb_ev.key == lili::Key::BACKSPACE)
                popLast(registry);
        }
    }
}

void
App::onRender(float alpha)
{
    // entities created are rendered with Game default class
    // if Render2DComponent.is_visible == true
    lili::Game::onRender(alpha);

    instructions_text.draw();
}
