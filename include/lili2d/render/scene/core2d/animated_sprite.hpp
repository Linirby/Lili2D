#pragma once

#include <cstdint>
#include <memory>
#include <string_view>

#include "lili2d/geometry/utils.hpp"
#include "lili2d/geometry/vec2.hpp"
#include "lili2d/render/interfaces/renderable.hpp"
#include "lili2d/render/renderer.hpp"
#include "lili2d/render/scene/common/animation.hpp"
#include "lili2d/render/scene/common/material.hpp"

namespace lili {

/// @brief A renderable 2D sprite driven by an Animation.
class AnimatedSprite : public IRenderable {
public:
    /// @brief Default constructor.
    AnimatedSprite() noexcept = default;
    /// @brief Constructs an animated sprite from an animation.
    /// @param renderer The renderer.
    /// @param animation The animation to play.
    explicit AnimatedSprite(Renderer* renderer, const Animation& animation);
    /// @brief Constructs an animated sprite from a registered animation key.
    /// @param renderer The renderer.
    /// @param animation_key The registry key of the animation.
    explicit AnimatedSprite(Renderer* renderer, std::string_view animation_key);
    /// @brief Constructs an animated sprite from a registered animation ID.
    /// @param renderer The renderer.
    /// @param animation_id The registry ID of the animation.
    explicit AnimatedSprite(Renderer* renderer, uint16_t animation_id);
    /// @brief Destructor.
    ~AnimatedSprite() override = default;

    /// @brief Move constructor.
    AnimatedSprite(AnimatedSprite&&) noexcept = default;
    /// @brief Move assignment operator.
    /// @return Reference to the assigned animated sprite.
    AnimatedSprite&
    operator=(AnimatedSprite&&) noexcept = default;

    /// @brief Sets the animation to play.
    /// @param animation The new animation.
    void
    setAnimation(const Animation& animation);
    /// @brief Sets the animation to play from a registry key.
    /// @param animation_key The registry key of the animation.
    void
    setAnimation(std::string_view animation_key);
    /// @brief Sets the animation to play from a registry ID.
    /// @param animation_id The registry ID of the animation.
    void
    setAnimation(uint16_t animation_id);

    /// @brief Sets the speed of the animation.
    /// @param speed_sec Time per frame in seconds.
    inline void
    setFrameSpeed(float speed_sec) noexcept {
        frame_speed_sec = speed_sec;
    }

    /// @brief Sets the sprite's color tint.
    /// @param color The new color tint.
    inline void
    setColorTint(Vec4 color) noexcept {
        if (material) material->properties.color_tint = color;
    }

    /// @brief Sets the sprite's color tint.
    /// @param color The new color tint.
    inline void
    setColor(Vec4 color) noexcept override {
        setColorTint(color);
    }

    /// @brief Sets the material pointer.
    /// @param mat Material pointer.
    inline void
    setMaterial(Material* mat) noexcept override {
        external_material = mat;
    }

    /// @brief Sets the sprite's position.
    /// @param pos The new position.
    inline void
    setPosition(Vec2 pos) noexcept override {
        this->position = pos;
        ui_layout.offset = pos;
    }

    /// @brief Sets the sprite's scale.
    /// @param s The new scale.
    inline void
    setScale(Vec2 s) noexcept override {
        this->scale = s;
    }

    /// @brief Sets the sprite's size (in px).
    /// @param s The new size.
    inline void
    setSize(Vec2 s) noexcept override {
        this->size = s;
    }

    /// @brief Sets the sprite's rotation.
    /// @param degree The rotation in degrees.
    inline void
    setRotation(float degree) noexcept override {
        rotation = lili::degToRad(degree);
    }

    /// @brief Sets the sprite's rendering layer depth.
    /// @param l The new layer depth.
    inline void
    setLayer(float l) noexcept override {
        this->layer = l;
    }

    /// @brief Get the position of the current frame.
    /// @return The position.
    [[nodiscard]] inline Vec2
    getPosition() const noexcept override {
        return position;
    }

    /// @brief Get rotation angle in degrees.
    /// @return Rotation in degrees.
    [[nodiscard]] inline float
    getRotation() const noexcept override {
        return lili::radToDeg(rotation);
    }

    /// @brief Get scale factors.
    /// @return Scale vector.
    [[nodiscard]] inline Vec2
    getScale() const noexcept override {
        return scale;
    }

    /// @brief Get the render width of the current frame.
    /// @return The width.
    [[nodiscard]] inline float
    getWidth() const noexcept {
        return size.x * scale.x;
    }

    /// @brief Get the render height of the current frame.
    /// @return The height.
    [[nodiscard]] inline float
    getHeight() const noexcept {
        return size.y * scale.y;
    }

    /// @brief Get the render size of the current frame.
    /// @return A 2D vector of the width and height.
    [[nodiscard]] inline Vec2
    getSize() const noexcept override {
        return Vec2(size.x * scale.x, size.y * scale.y);
    }

    /// @brief Gets transformation matrix.
    /// @return Mat3 matrix.
    [[nodiscard]] Mat3
    getTransformMatrix() const override;

    /// @brief Gets layer depth.
    /// @return Depth layer.
    [[nodiscard]] inline float
    getLayer() const noexcept override {
        return layer;
    }

    /// @brief Gets color tint.
    /// @return Color tint.
    [[nodiscard]] inline Vec4
    getColor() const noexcept override {
        Material* mat = getMaterial();
        return mat ? mat->properties.color_tint : Vec4(1.0f, 1.0f, 1.0f, 1.0f);
    }

    /// @brief Gets the material.
    /// @return Pointer to the material.
    [[nodiscard]] inline Material*
    getMaterial() const noexcept override {
        return external_material ? external_material : material.get();
    }

    /// @brief Advances the animation by dt seconds.
    /// @param dt Delta time in seconds.
    void
    update(float dt);
    /// @brief Resets the animation to the FIRST frame.
    void
    reset() noexcept;
    /// @brief Submits the sprite for drawing.
    void
    draw() override;

private:
    /// @brief Pointer to parent Renderer.
    Renderer* renderer = nullptr;

    /// @brief Pointer to GPU mesh.
    GPUMesh* mesh = nullptr;
    /// @brief Unique pointer to owned Material.
    std::unique_ptr<Material> material = nullptr;
    /// @brief Pointer to external unowned Material.
    Material* external_material = nullptr;

    /// @brief Played animation object.
    Animation animation;
    /// @brief Current frame index.
    size_t current_frame = 0;
    /// @brief Frame duration speed in seconds per frame.
    float frame_speed_sec = 0.1f;
    /// @brief Accumulated time spent on current frame.
    float frame_time_sec = 0.0f;

    /// @brief Sprite 2D position.
    Vec2 position;
    /// @brief Sprite 2D scale.
    Vec2 scale = {1.0f, 1.0f};
    /// @brief Sprite 2D render size.
    Vec2 size = {1.0f, 1.0f};
    /// @brief Sprite rotation in degrees.
    float rotation = 0.0f;
    /// @brief Render layer depth.
    float layer = 0.0f;

    /// @brief Applies a specific slice frame to the sprite's mesh.
    /// @param frame The frame to apply.
    void
    applyFrame(const SliceUV& frame);
};

}  // namespace lili
