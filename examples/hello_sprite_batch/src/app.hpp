#pragma once

#include "lili2d/geometry/vec2.hpp"
#include "lili2d/render/common/atlas_map.hpp"
#include <lili2d/lili2d.hpp>
#include <memory>
#include <vector>

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
