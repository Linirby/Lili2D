#pragma once

#include <lili2d/core/game.hpp>

class App : public lili::Game
{
public:
    App();

private:
    lili::Camera camera;

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
