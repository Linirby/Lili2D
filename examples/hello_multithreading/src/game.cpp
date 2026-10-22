#include "game.hpp"

#include "components.hpp"
#include "systems.hpp"
#include <iostream>
#include <lili2d/ecs/ecs_registry.hpp>
#include <lili2d/entities/sprite.hpp>
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

void
App::onInit()
{
    for (int i = 0; i < N_ENTITIES; ++i)
        spawnRandomBall();

    std::cout << "Instructions\n"
              << "SPACE: Spawn a new random ball entity\n"
              << "BACKSPACE: Destroy a random ball entity\n"
              << "M: Toggle Multithreading\n";
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
            else if (kb.key == Key::M)
                toggleMultithreading();
            std::cout << "Total entities: " << spawned_entities.size() << "\n";
        }
    }
}

void
App::onUpdate(float dt)
{
    Vec2 window_size = getWindow()->getSize();
    MovementSystem::update(getECSRegistry(), window_size, dt, getThreadPool());
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
    std::uniform_real_distribution<float> disScale(0.4f, 1.2f);
    std::uniform_real_distribution<float> disColor(0.2f, 1.0f);

    Vec2 pos(disX(gen), disY(gen));
    Vec2 vel(disVel(gen), disVel(gen));
    float scale = disScale(gen);
    Vec4 color(disColor(gen), disColor(gen), disColor(gen), 1.0f);

    ECSRegistry& registry = getECSRegistry();
    Entity ent = createSprite(
        registry,
        getRenderer(),
        "circle.png",
        { .color = color, .pos = pos, .scale = { scale, scale } }
    );
    registry.emplaceComponent<VelocityComponent>(ent, vel);
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

    getECSRegistry().destroyEntity(ent);
    spawned_entities.erase(spawned_entities.begin() + index);
}

void
App::toggleMultithreading()
{
    EngineConfig current = getConfig();
    if (current.profile == PerformanceProfile::YES) {
        current.profile = PerformanceProfile::CORRECT;
        configure(current);
        std::cout << ">> Switch to Profile: CORRECT"
                     " (Multithreaded ECS + Mailbox present)\n";
    } else {
        current.profile = PerformanceProfile::YES;
        current.potato_present_mode = SDL_GPU_PRESENTMODE_VSYNC;
        configure(current);
        std::cout << ">> Switch to Profile: YES"
                     " (Single-threaded ECS + VSync present)\n";
    }
}
