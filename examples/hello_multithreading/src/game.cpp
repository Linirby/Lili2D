#include "game.hpp"

#include "components.hpp"
#include "systems.hpp"
#include <iostream>
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
  : Game("hello_multithreading - Lili2D", 800, 600)
{
    setTps(60.0f);
}

void
App::onInit()
{
    Window* window = getWindow();
    Renderer* renderer = getRenderer();

    camera = Camera();
    camera.setPosition(
        { window->getWidth() / 2.0f, window->getHeight() / 2.0f }
    );
    renderer->setCamera(&camera);

    circle_texture =
        Assets::loadTexture("circle_tex", "circle.png", renderer->getDevice());
    dynamic_batch = std::make_unique<DynamicBatch>(renderer, circle_texture);

    movement_system = std::make_unique<systems::MovementSystem>(
        static_cast<float>(window->getWidth()),
        static_cast<float>(window->getHeight())
    );

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
    movement_system->run(ecs_registry, dt, getThreadPool());
}

void
App::onRender(float alpha)
{
    Game::onRender(alpha);

    dynamic_batch->begin();

    auto& pos_pool = ecs_registry.getPool<PositionComponent>();
    const auto& entities = pos_pool.getEntities();
    const auto& positions = pos_pool.getComponents();

    for (size_t i = 0; i < entities.size(); ++i) {
        Entity entity = entities[i];
        if (ecs_registry.hasComponent<RenderComponent>(entity)) {
            const auto& pos = positions[i];
            auto& render = ecs_registry.getComponent<RenderComponent>(entity);

            dynamic_batch->addSprite(
                render.slice,
                pos.value,
                { render.radius * 2.0f, render.radius * 2.0f },
                0.0f,
                render.color
            );
        }
    }

    dynamic_batch->flush(0.0f, RenderLayer::WORLD2D);
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
    std::uniform_real_distribution<float> disRadius(5.0f, 15.0f);
    std::uniform_real_distribution<float> disColor(0.2f, 1.0f);

    Vec2 pos(disX(gen), disY(gen));
    Vec2 vel(disVel(gen), disVel(gen));
    float radius = disRadius(gen);
    Vec4 color(disColor(gen), disColor(gen), disColor(gen), 1.0f);

    Entity ent = ecs_registry.createEntity();
    ecs_registry.emplaceComponent<PositionComponent>(ent, pos);
    ecs_registry.emplaceComponent<VelocityComponent>(ent, vel);
    ecs_registry.emplaceComponent<RenderComponent>(
        ent,
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
