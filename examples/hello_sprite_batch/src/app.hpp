#pragma once

#include <lili2d/core/game.hpp>
#include <lili2d/core/keyboard.hpp>
#include <lili2d/render/2d/static_batch.hpp>
#include <lili2d/render/2d/text.hpp>
#include <lili2d/render/common/animation_player.hpp>
#include <lili2d/render/common/atlas_map.hpp>

struct Character
{
    lili::Vec2 position;
    lili::AnimationPlayer anim_player;
};

struct TileDrawItem
{
    lili::SliceUV slice;
    lili::Vec2 pos;
};

class App : public lili::Game
{
public:
    App();

private:
    lili::Camera camera;
    lili::Keyboard keyboard;

    lili::AtlasMap* env_atlas = nullptr;
    lili::AtlasMap* char_atlas = nullptr;

    std::unique_ptr<lili::StaticBatch> static_batch;
    std::unique_ptr<lili::DynamicBatch> dynamic_batch;

    std::vector<TileDrawItem> tiles_draw_data;
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
