#pragma once

#include <SDL3/SDL.h>

#include <lili2d/lili2d.hpp>
#include <memory>

class App : public lili::Game {
public:
    App();

private:
    lili::Camera camera;
    lili::Keyboard keyboard;

    lili::AtlasMap* env_atlas = nullptr;

    std::unique_ptr<lili::ThreadPool> thread_pool;
    std::unique_ptr<lili::TileMap> tilemap = nullptr;

    lili::Text text_infos;

    void
    onEvent(const lili::Event& event) override;
    void
    onUpdate(float dt) override;
    void
    onRender(float alpha) override;
};
