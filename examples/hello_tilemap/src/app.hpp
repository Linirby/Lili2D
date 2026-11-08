#pragma once

#include <lili2d/core/game.hpp>
#include <lili2d/core/keyboard.hpp>
#include <lili2d/render/common/atlas_map.hpp>
#include <lili2d/world/tilemap.hpp>

class App : public lili::Game
{
public:
    App();

private:
    lili::Camera camera;
    lili::Keyboard keyboard;

    lili::AtlasMap* env_atlas = nullptr;
    std::unique_ptr<lili::TileMap> tilemap;

    lili::Entity text_infos;

    void
    onEvent(const lili::Event& event) override;
    void
    onUpdate(float dt) override;
    void
    onRender(float alpha) override;
};
