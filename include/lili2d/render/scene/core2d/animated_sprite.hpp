#pragma once

#include <cstdint>
#include <memory>
#include <string>

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
    AnimatedSprite() = default;
    /// @brief Constructs an animated sprite from an animation.
    /// @param renderer The renderer.
    /// @param animation The animation to play.
    AnimatedSprite(Renderer* renderer, const Animation& animation);
    /// @brief Constructs an animated sprite from a registered animation key.
    /// @param renderer The renderer.
    /// @param animation_key The registry key of the animation.
    AnimatedSprite(Renderer* renderer, const std::string& animation_key);
    /// @brief Constructs an animated sprite from a registered animation ID.
    /// @param renderer The renderer.
    /// @param animation_id The registry ID of the animation.
    AnimatedSprite(Renderer* renderer, uint16_t animation_id);
    /// @brief Destructor.
    ~AnimatedSprite() override = default;

    /// @brief Move constructor.
    AnimatedSprite(AnimatedSprite&&) = default;
    /// @brief Move assignment operator.
    /// @return Reference to the assigned animated sprite.
    AnimatedSprite&
    operator=(AnimatedSprite&&) = default;

    /// @brief Sets the animation to play.
    /// @param animation The new animation.
    void
    setAnimation(const Animation& animation);
    /// @brief Sets the animation to play from a registry key.
    /// @param animation_key The registry key of the animation.
    void
    setAnimation(const std::string& animation_key);
    /// @brief Sets the animation to play from a registry ID.
    /// @param animation_id The registry ID of the animation.
    void
    setAnimation(uint16_t animation_id);

    /// @brief Sets the speed of the animation.
    /// @param speed_sec Time per frame in seconds.
    void
    setFrameSpeed(float speed_sec);

    /// @brief Sets the sprite's color tint.
    /// @param color The new color tint.
    void
    setColorTint(const Vec4& color);
    /// @brief Sets the sprite's color tint.
    /// @param color The new color tint.
    void
    setColor(Vec4 color) override;
    /// @brief Sets the material pointer.
    /// @param material Material pointer.
    void
    setMaterial(Material* material) override;
    /// @brief Sets the sprite's position.
    /// @param position The new position.
    void
    setPosition(Vec2 position) override;
    /// @brief Sets the sprite's scale.
    /// @param scale The new scale.
    void
    setScale(Vec2 scale) override;
    /// @brief Sets the sprite's size (in px).
    /// @param size The new size.
    void
    setSize(Vec2 size) override;
    /// @brief Sets the sprite's rotation.
    /// @param degree The rotation in degrees.
    void
    setRotation(float degree) override;
    /// @brief Sets the sprite's rendering layer depth.
    /// @param layer The new layer depth.
    void
    setLayer(float layer) override;
    /// @brief Sets the render pass layer.
    /// @param render_layer Render pass layer.
    void
    setRender(RenderLayer render_layer) override;

    /// @brief Get the position of the current frame.
    /// @return The position.
    Vec2
    getPosition() const override;
    /// @brief Get rotation angle in degrees.
    /// @return Rotation in degrees.
    float
    getRotation() const override;
    /// @brief Get scale factors.
    /// @return Scale vector.
    Vec2
    getScale() const override;
    /// @brief Get the render width of the current frame.
    /// @return The width.
    float
    getWidth() const;
    /// @brief Get the render height of the current frame.
    /// @return The height.
    float
    getHeight() const;
    /// @brief Get the render size of the current frame.
    /// @return A 2D vector of the width and height.
    Vec2
    getSize() const override;
    /// @brief Gets transformation matrix.
    /// @return Mat3 matrix.
    Mat3
    getTransformMatrix() const override;
    /// @brief Gets layer depth.
    /// @return Depth layer.
    float
    getLayer() const override;
    /// @brief Gets render pass layer.
    /// @return Render pass layer.
    RenderLayer
    getRender() const override;
    /// @brief Gets color tint.
    /// @return Color tint.
    Vec4
    getColor() const override;
    /// @brief Gets the material.
    /// @return Pointer to the material.
    Material*
    getMaterial() const override;

    /// @brief Advances the animation by dt seconds.
    /// @param dt Delta time in seconds.
    void
    update(float dt);
    /// @brief Resets the animation to the FIRST frame.
    void
    reset();
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
    /// @brief Target render pass layer.
    RenderLayer render_layer = RenderLayer::WORLD2D;

    /// @brief Applies a specific slice frame to the sprite's mesh.
    /// @param frame The frame to apply.
    void
    applyFrame(const SliceUV& frame);
};

}  // namespace lili
