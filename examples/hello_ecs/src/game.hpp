#pragma once

#include <lili2d/core/game.hpp>
#include <lili2d/ecs/ecs_registry.hpp>
#include <lili2d/render/core/texture.hpp>
#include <lili2d/render/scene/core2d/sprite_batch.hpp>
#include <lili2d/world/camera.hpp>

class App : public lili::Game {
public:
    App();

private:
    lili::Camera camera;
    lili::ECSRegistry ecs_registry;

    lili::Texture* circle_texture = nullptr;
    std::unique_ptr<lili::SpriteBatch> sprite_batch;
    std::vector<lili::Entity> spawned_entities;

    static constexpr int N_ENTITIES = 1000;

    void
    onEvent(const lili::Event& event) override;
    void
    onUpdate(float dt) override;
    void
    onRender(float alpha) override;

    void
    spawnRandomBall();
    void
    destroyRandomBall();
    void
    toggleRandomBallVelocity();
};
