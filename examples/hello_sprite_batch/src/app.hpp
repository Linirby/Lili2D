#pragma once

#include <SDL3/SDL.h>

#include <lili2d/lili2d.hpp>
#include <memory>

struct Tile {
    lili::Vec2 position;
    lili::SliceUV slice;
};

struct Character {
    lili::Vec2 position;
    lili::AnimationPlayer anim_player;
};

class App : public lili::Game {
public:
    App();

private:
    lili::Camera camera;
    lili::Keyboard keyboard;

    lili::AtlasMap* env_atlas = nullptr;
    std::unique_ptr<lili::SpriteBatch> env_batch;
    lili::AtlasMap* char_atlas = nullptr;
    std::unique_ptr<lili::SpriteBatch> char_batch;

    Character player;

    lili::Animation anim_idle;
    lili::Animation anim_run_right;
    lili::Animation anim_run_left;
    lili::Animation anim_run_top;
    lili::Animation anim_run_bottom;
    lili::Animation* current_anim = nullptr;

    lili::Text text_infos;

    void
    onEvent(const lili::Event& event) override;
    void
    onUpdate(float dt) override;
    void
    onRender(float alpha) override;
};
