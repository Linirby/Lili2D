#include "game.hpp"

#include <SDL3/SDL_keycode.h>

#include <iostream>
#include <random>

#include "components.hpp"
#include "entities.hpp"
#include "systems.hpp"

namespace {
std::mt19937&
getRng() {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    return gen;
}
}  // namespace

App::App() : lili::Game("hello_ecs - Lili2D", 800, 600) {
    setTps(20.0f);
    lili::Window* window = getWindow();
    lili::Renderer* renderer = getRenderer();

    window->setResizable(true);

    camera = lili::Camera();
    camera.setPosition({window->getWidth() / 2.0f, window->getHeight() / 2.0f});
    renderer->setCamera(&camera);

    circle_texture = lili::Assets::loadTexture(
        "circle_tex", "circle.png", renderer->getDevice()
    );
    sprite_batch =
        std::make_unique<lili::SpriteBatch>(renderer, circle_texture);
    for (int i = 0; i < N_ENTITIES; ++i) spawnRandomBall();

    std::cout
        << "=== Lili2D ECS Demo Instructions ===\n"
        << "  [SPACE]     : Spawn a new random ball entity\n"
        << "  [BACKSPACE] : Destroy a random ball entity\n"
        << "  [T]         : Toggle velocity component of a random ball\n"
        << "====================================\n";
}

void
App::onEvent(const lili::Event& event) {
    lili::Game::onEvent(event);
    if (event.type() == lili::EventType::KEYBOARD) {
        lili::KeyboardEvent kb = event.keyboard();
        if (kb.action == lili::KeyAction::PRESSED) {
            if (kb.key == SDLK_ESCAPE) shutdown();
            else if (kb.key == SDLK_SPACE) spawnRandomBall();
            else if (kb.key == SDLK_BACKSPACE) destroyRandomBall();
            else if (kb.key == SDLK_T) toggleRandomBallVelocity();
        }
    }
}

void
App::onUpdate(float dt) {
    lili::Window* window = getWindow();
    camera.setPosition({window->getWidth() / 2.0f, window->getHeight() / 2.0f});
    systems::updateMovement(
        ecs_registry, dt, static_cast<float>(window->getWidth()),
        static_cast<float>(window->getHeight())
    );
}

void
App::onRender(float alpha) {
    (void)alpha;
    systems::renderEntities(ecs_registry, *sprite_batch);
}

void
App::spawnRandomBall() {
    auto& gen = getRng();
    lili::Window* window = getWindow();
    std::uniform_real_distribution<float> disX(
        50.0f, static_cast<float>(window->getWidth()) - 50.0f
    );
    std::uniform_real_distribution<float> disY(
        50.0f, static_cast<float>(window->getHeight()) - 50.0f
    );
    std::uniform_real_distribution<float> disVel(-200.0f, 200.0f);
    std::uniform_real_distribution<float> disRadius(10.0f, 30.0f);
    std::uniform_real_distribution<float> disColor(0.2f, 1.0f);

    lili::Vec2 pos(disX(gen), disY(gen));
    lili::Vec2 vel(disVel(gen), disVel(gen));
    float radius = disRadius(gen);
    lili::Vec4 color(disColor(gen), disColor(gen), disColor(gen), 1.0f);

    lili::Entity ent = entities::spawnBall(
        ecs_registry, pos, vel,
        lili::SliceUV(circle_texture, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f),
        color, radius
    );
    spawned_entities.push_back(ent);
}

void
App::destroyRandomBall() {
    if (spawned_entities.empty()) return;

    auto& gen = getRng();
    std::uniform_int_distribution<size_t> dis(0, spawned_entities.size() - 1);
    size_t index = dis(gen);
    lili::Entity ent = spawned_entities[index];

    ecs_registry.destroyEntity(ent);
    spawned_entities.erase(spawned_entities.begin() + index);
}

void
App::toggleRandomBallVelocity() {
    if (spawned_entities.empty()) return;

    auto& gen = getRng();
    std::uniform_int_distribution<size_t> dis(0, spawned_entities.size() - 1);
    size_t index = dis(gen);
    lili::Entity ent = spawned_entities[index];

    if (ecs_registry.hasComponent<VelocityComponent>(ent)) {
        ecs_registry.removeComponent<VelocityComponent>(ent);
    } else {
        std::uniform_real_distribution<float> disVel(-200.0f, 200.0f);
        ecs_registry.emplaceComponent<VelocityComponent>(
            ent, lili::Vec2(disVel(gen), disVel(gen))
        );
    }
}
