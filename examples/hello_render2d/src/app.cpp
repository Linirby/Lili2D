#include "app.hpp"
#include "rng_helper.hpp"

#include <cstddef>
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
#include <memory>
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
    lili::Vec4 color
)
{
    lili::Entity e =
        lili::createRect(registry, pos, size, { 1.0f, 1.0f }, 0.0f, color);
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

    for (size_t i = 0; i < N_ENTITIES / 2.0f; i++) {
        addBaguette(registry, renderer, rng::pos());
        addRect(registry, rng::pos(), rng::size(), rng::color());
    }

    batch = std::make_unique<lili::DynamicBatch>(getRenderer(), nullptr);
}

void
App::onEvent(const lili::Event& event)
{
    lili::ECSRegistry& registry = getECSRegistry();
    lili::Renderer* renderer = getRenderer();

    if (event.type() == lili::EventType::KEYBOARD) {
        lili::KeyboardEvent kb_ev = event.keyboard();
        if (event.keyJustPressed(kb_ev)) {
            if (kb_ev.key == lili::Key::ESCAPE)
                shutdown();
            else if (kb_ev.key == lili::Key::NUM_1)
                addBaguette(registry, renderer, rng::pos());
            else if (kb_ev.key == lili::Key::NUM_2)
                addRect(registry, rng::pos(), rng::size(), rng::color());
            else if (kb_ev.key == lili::Key::BACKSPACE)
                popLast(registry);
        }
    }
}

void
App::onRender(float alpha)
{
    lili::Render2DSystem::render(
        getECSRegistry(), *batch, getRenderer()->getTheWhitePixel()
    );
}
