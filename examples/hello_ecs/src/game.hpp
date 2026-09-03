#pragma once

#include <lili2d/lili2d.hpp>

class App : public lili::Game {
public:
    App();

private:
    lili::Camera camera;
    lili::ECSRegistry ecs_registry;

    lili::Texture* circle_texture = nullptr;
    std::unique_ptr<lili::SpriteBatch> sprite_batch;
    std::vector<lili::Entity> spawned_entities;

    static constexpr int N_ENTITIES = 10000;

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
