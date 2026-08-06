#pragma once

#include <SDL3/SDL.h>

#include <lili2d/lili2d.hpp>
#include <memory>

/// @brief Tile data for sprite batching.
struct Tile {
    lili::Vec2 position;  ///< Tile position.
    lili::SliceUV slice;  ///< Tile UV slice.
};

/// @brief Character data for sprite batching.
struct Character {
    lili::Vec2 position;                ///< Character position.
    lili::AnimationPlayer anim_player;  ///< Character animation player.
};

/// @brief Application class for sprite batching demonstration.
class App : public lili::Game {
public:
    /// @brief Default constructor initializing the App.
    App();

private:
    /// @brief 2D camera.
    lili::Camera camera;
    /// @brief Keyboard input tracker.
    lili::Keyboard keyboard;

    /// @brief Environment atlas map.
    lili::AtlasMap* env_atlas = nullptr;
    /// @brief Environment sprite batch.
    std::unique_ptr<lili::SpriteBatch> env_batch;
    /// @brief Character atlas map.
    lili::AtlasMap* char_atlas = nullptr;
    /// @brief Character sprite batch.
    std::unique_ptr<lili::SpriteBatch> char_batch;

    /// @brief Player character instance.
    Character player;

    /// @brief Idle animation.
    lili::Animation anim_idle;
    /// @brief Run right animation.
    lili::Animation anim_run_right;
    /// @brief Run left animation.
    lili::Animation anim_run_left;
    /// @brief Run top animation.
    lili::Animation anim_run_top;
    /// @brief Run bottom animation.
    lili::Animation anim_run_bottom;
    /// @brief Pointer to current playing animation.
    lili::Animation* current_anim = nullptr;

    /// @brief Debug info text.
    lili::Text text_infos;

    /// @brief Event callback.
    /// @param event Incoming event.
    void
    onEvent(const lili::Event& event) override;
    /// @brief Frame update callback.
    /// @param dt Delta time in seconds.
    void
    onUpdate(float dt) override;
    /// @brief Frame render callback.
    /// @param alpha Interpolation factor.
    void
    onRender(float alpha) override;
};
