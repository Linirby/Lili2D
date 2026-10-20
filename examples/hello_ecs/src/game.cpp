#include "game.hpp"

#include "components.hpp"
#include "entities.hpp"
#include "systems.hpp"
#include <iostream>
#include <lili2d/core/asset_manager.hpp>
#include <lili2d/core/event.hpp>
#include <lili2d/core/window.hpp>
#include <lili2d/render/renderer.hpp>
#include <memory>
#include <random>

using namespace lili;

namespace {
std::mt19937&
getRng()
{
    static std::random_device rd;
    static std::mt19937 gen(rd());
    return gen;
}
} // namespace

App::App()
  : Game("hello_ecs - Lili2D", 800, 600)
{
    setTps(20.0f);
    Window* window = getWindow();
    Renderer* renderer = getRenderer();

    window->setResizable(true);

    camera = Camera();
    camera.setPosition(
        { window->getWidth() / 2.0f, window->getHeight() / 2.0f }
    );
    renderer->setCamera(&camera);

    circle_texture =
        Assets::loadTexture("circle_tex", "circle.png", renderer->getDevice());
    dynamic_batch = std::make_unique<DynamicBatch>(renderer, circle_texture);
    for (int i = 0; i < N_ENTITIES; ++i)
        spawnRandomBall();

    std::cout << "Instructions\n"
              << "SPACE: Spawn a new random ball entity\n"
              << "BACKSPACE: Destroy a random ball entity\n"
              << "T: Toggle velocity component of a random ball\n";
}

void
App::onEvent(const Event& event)
{
    Game::onEvent(event);

    if (event.type() == EventType::KEYBOARD) {
        KeyboardEvent kb = event.keyboard();
        if (kb.action == KeyAction::PRESSED) {
            if (kb.key == Key::ESCAPE)
                shutdown();
            else if (kb.key == Key::SPACE)
                spawnRandomBall();
            else if (kb.key == Key::BACKSPACE)
                destroyRandomBall();
            else if (kb.key == Key::T)
                toggleRandomBallVelocity();
            std::cout << "Total entities: " << spawned_entities.size() << "\n";
        }
    }
}

void
App::onUpdate(float dt)
{
    Game::onUpdate(dt);

    Window* window = getWindow();
    camera.setPosition(
        { window->getWidth() / 2.0f, window->getHeight() / 2.0f }
    );
    systems::updateMovement(
        ecs_registry,
        dt,
        static_cast<float>(window->getWidth()),
        static_cast<float>(window->getHeight())
    );
}

void
App::onRender(float alpha)
{
    Game::onRender(alpha);

    systems::renderEntities(ecs_registry, *dynamic_batch);
}

void
App::spawnRandomBall()
{
    auto& gen = getRng();
    Window* window = getWindow();
    std::uniform_real_distribution<float> disX(
        50.0f, static_cast<float>(window->getWidth()) - 50.0f
    );
    std::uniform_real_distribution<float> disY(
        50.0f, static_cast<float>(window->getHeight()) - 50.0f
    );
    std::uniform_real_distribution<float> disVel(-200.0f, 200.0f);
    std::uniform_real_distribution<float> disRadius(10.0f, 30.0f);
    std::uniform_real_distribution<float> disColor(0.2f, 1.0f);

    Vec2 pos(disX(gen), disY(gen));
    Vec2 vel(disVel(gen), disVel(gen));
    float radius = disRadius(gen);
    Vec4 color(disColor(gen), disColor(gen), disColor(gen), 1.0f);

    Entity ent = entities::spawnBall(
        ecs_registry,
        pos,
        vel,
        SliceUV(circle_texture, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f),
        color,
        radius
    );
    spawned_entities.push_back(ent);
}

void
App::destroyRandomBall()
{
    if (spawned_entities.empty())
        return;

    auto& gen = getRng();
    std::uniform_int_distribution<size_t> dis(0, spawned_entities.size() - 1);
    size_t index = dis(gen);
    Entity ent = spawned_entities[index];

    ecs_registry.destroyEntity(ent);
    spawned_entities.erase(spawned_entities.begin() + index);
}

void
App::toggleRandomBallVelocity()
{
    if (spawned_entities.empty())
        return;

    auto& gen = getRng();
    std::uniform_int_distribution<size_t> dis(0, spawned_entities.size() - 1);
    size_t index = dis(gen);
    Entity ent = spawned_entities[index];

    if (ecs_registry.hasComponent<VelocityComponent>(ent)) {
        ecs_registry.removeComponent<VelocityComponent>(ent);
    } else {
        std::uniform_real_distribution<float> disVel(-200.0f, 200.0f);
        ecs_registry.emplaceComponent<VelocityComponent>(
            ent, Vec2(disVel(gen), disVel(gen))
        );
    }
}
