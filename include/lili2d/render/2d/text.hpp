#pragma once

#include <memory>
#include <string>

#include "lili2d/geometry/utils.hpp"
#include "lili2d/geometry/vec2.hpp"
#include "lili2d/render/2d/bitmap_font.hpp"
#include "lili2d/render/common/material.hpp"
#include "lili2d/render/common/model.hpp"
#include "lili2d/render/common/renderable.hpp"
#include "lili2d/render/gpu/gpu_mesh.hpp"

namespace lili {

/// @brief Renderable Text using a bitmap font.
class [[deprecated("Use TextComponent with ECS instead")]] Text
  : public IRenderable
{
public:
    /// @brief Default constructor.
    Text() noexcept = default;
    /// @brief Constructs text using a bitmap font.
    /// @param renderer The renderer.
    /// @param font Pointer to the bitmap font.
    /// @param text The text string to render.
    explicit Text(
        Renderer* renderer,
        BitmapFont* font,
        const std::string& text
    );
    /// @brief Destructor.
    ~Text() override = default;

    /// @brief Move constructor.
    Text(Text&&) noexcept = default;
    /// @brief Move assignment operator.
    /// @return Reference to the assigned text.
    Text&
    operator=(Text&&) noexcept = default;

    /// @brief Sets the text string.
    /// @param value The new text string.
    void
    setText(const std::string& value);
    /// @brief Sets the text's position.
    /// @param position The new position.
    inline void
    setPosition(Vec2 position) noexcept override
    {
        pos = position;
        ui_layout.offset = position;
    }
    /// @brief Sets rotation in degrees.
    /// @param degree Rotation in degrees.
    inline void
    setRotation(float degree) noexcept override
    {
        rotation = lili::degToRad(degree);
    }
    /// @brief Sets scale factors.
    /// @param scale The new scale.
    inline void
    setScale(Vec2 scale) noexcept override
    {
        this->scale = scale;
    }
    /// @brief Sets uniform scale.
    /// @param value The scale value.
    inline void
    setScale(float value) noexcept
    {
        this->scale = Vec2(value, value);
    }
    /// @brief Sets text bounds size.
    /// @param size The size.
    inline void
    setSize(Vec2 size) noexcept override
    {
        (void)size;
    }
    /// @brief Sets the spacing between characters.
    /// @param value The spacing value.
    void
    setSpacing(float value);
    /// @brief Sets text color tint.
    /// @param color Color tint.
    inline void
    setColor(Vec4 color) noexcept override
    {
        if (material) {
            material->properties.color_tint = color;
        }
    }
    /// @brief Sets material.
    /// @param material Material pointer.
    inline void
    setMaterial(Material* material) noexcept override
    {
        external_material = material;
    }
    /// @brief Sets layer depth.
    /// @param layer Layer depth.
    inline void
    setLayer(float layer) noexcept override
    {
        this->layer = layer;
    }

    /// @brief Gets position.
    /// @return Position.
    [[nodiscard]] inline Vec2
    getPosition() const noexcept override
    {
        return pos;
    }
    /// @brief Gets rotation in degrees.
    /// @return Rotation in degrees.
    [[nodiscard]] inline float
    getRotation() const noexcept override
    {
        return lili::radToDeg(rotation);
    }
    /// @brief Gets scale.
    /// @return Scale vector.
    [[nodiscard]] inline Vec2
    getScale() const noexcept override
    {
        return scale;
    }
    /// @brief Gets bounding size of text.
    /// @return Size vector.
    [[nodiscard]] inline Vec2
    getSize() const noexcept override
    {
        float width = text.length() * advance * scale.x;
        float height = glyph_h * scale.y;
        return { width, height };
    }
    /// @brief Gets transformation matrix.
    /// @return Mat3 transform matrix.
    [[nodiscard]] Mat3
    getTransformMatrix() const override;
    /// @brief Gets layer depth.
    /// @return Depth layer.
    [[nodiscard]] inline float
    getLayer() const noexcept override
    {
        return layer;
    }
    /// @brief Gets color tint.
    /// @return Color tint vector.
    [[nodiscard]] inline Vec4
    getColor() const noexcept override
    {
        Material* mat = getMaterial();
        return mat ? mat->properties.color_tint : Vec4(1, 1, 1, 1);
    }
    /// @brief Gets the material.
    /// @return Pointer to the material.
    [[nodiscard]] inline Material*
    getMaterial() const noexcept override
    {
        return external_material ? external_material : material.get();
    }

    /// @brief Submits the text for drawing.
    void
    draw() override;

private:
    Renderer* renderer = nullptr;
    BitmapFont* font = nullptr;

    std::string text = "default";
    Vec2 pos;

    float rotation = 0.0f;
    Vec2 scale = { 1.0f, 1.0f };
    float glyph_w = 1.0f;
    float glyph_h = 1.0f;
    float advance = 1.0f;

    std::unique_ptr<Material> material = nullptr;
    Material* external_material = nullptr;
    std::unique_ptr<GPUMesh> mesh = nullptr;
    Model model;
    float layer = 0.0f;

    void
    rebuildMesh();
};

} // namespace lili
