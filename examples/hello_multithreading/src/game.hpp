#pragma once

#include <lili2d/lili2d.hpp>
#include <vector>

class App : public lili::Game
{
public:
    App()
      : Game("hello_multithreading - Lili2D", 800, 600)
    {
        setTps(60.0f);
    }
    ~App() override = default;

    void
    onInit() override;
    void
    onEvent(const lili::Event& event) override;
    void
    onUpdate(float dt) override;

private:
    void
    spawnRandomBall();
    void
    destroyRandomBall();
    void
    toggleMultithreading();

    static constexpr int N_ENTITIES = 10000;

    std::vector<lili::Entity> spawned_entities;
};
