#include "lili2d/render/scene/common/text.hpp"

#include "lili2d/geometry/mat3x3.hpp"
#include "lili2d/geometry/utils.hpp"
#include "lili2d/render/renderer.hpp"

namespace lili {

BitmapFont::BitmapFont(
    Renderer* renderer, const std::string& path, uint8_t cols, uint8_t rows
)
    : cols(cols), rows(rows) {
    texture = std::make_unique<Texture>(renderer->getDevice(), path);
    this->cols = cols;
    this->rows = rows;
    glyph_w = texture->getWidth() / cols;
    glyph_h = texture->getHeight() / rows;
}

BitmapFont::BitmapFont(BitmapFont&& other) noexcept
    : texture(std::move(other.texture)),
      cols(other.cols),
      rows(other.rows),
      glyph_w(other.glyph_w),
      glyph_h(other.glyph_h) {}

BitmapFont&
BitmapFont::operator=(BitmapFont&& other) noexcept {
    if (this != &other) {
        if (texture && other.texture)
            *texture = std::move(*other.texture);
        else
            texture = std::move(other.texture);
        cols = other.cols;
        rows = other.rows;
        glyph_w = other.glyph_w;
        glyph_h = other.glyph_h;
    }
    return *this;
}

GlyphUV
BitmapFont::glyphUv(char c) const noexcept {
    const int ASCII = static_cast<unsigned char>(c);
    const int FIRST = static_cast<unsigned char>(' ');
    int idx = ASCII - FIRST;
    if (ASCII < FIRST || ASCII > 126) idx = '?' - FIRST;
    const int CURRENT_X = idx % cols;
    const int CURRENT_Y = idx / cols;
    const float DELTA_U = 1.0f / static_cast<float>(cols);
    const float DELTA_V = 1.0f / static_cast<float>(rows);

    float tex_w = static_cast<float>(texture->getWidth());
    float tex_h = static_cast<float>(texture->getHeight());

    float eps_u = 0.5f / tex_w;
    float eps_v = 0.5f / tex_h;

    float u0 = CURRENT_X * DELTA_U + eps_u;
    float v0 = CURRENT_Y * DELTA_V + eps_v;
    float u1 = (CURRENT_X + 1) * DELTA_U - eps_u;
    float v1 = (CURRENT_Y + 1) * DELTA_V - eps_v;

    return GlyphUV(u0, v0, u1, v1);
}

Text::Text(Renderer* renderer, BitmapFont* font, const std::string& text) {
    this->renderer = renderer;
    this->font = font;
    glyph_w = static_cast<float>(font->getGlyphW());
    glyph_h = static_cast<float>(font->getGlyphH());
    advance = glyph_w + 1.0f;
    if (!text.empty())
        this->text = text;
    else
        this->text = "text";
    material = std::make_unique<Material>(font->getTexture());
    material->properties.color_tint = {1.0f, 1.0f, 1.0f, 1.0f};
    pos = {0.0f, 0.0f};
    ui_layout.offset = {0.0f, 0.0f};
    rebuildMesh();
}

void
Text::setText(const std::string& value) {
    if (value == text) return;
    text = value;
    rebuildMesh();
}

void
Text::setSpacing(float value) {
    advance = glyph_w + value;
    rebuildMesh();
}

Mat3
Text::getTransformMatrix() const {
    if (render_layer == RenderLayer::UI && renderer) {
        Vec2 viewport_size = renderer->getLogicalResolution();
        return ui_layout.getTransformationMatrix(
            viewport_size, getSize(), rotation, scale
        );
    }
    return Mat3::translate(pos) * Mat3::rotation(rotation) * Mat3::scale(scale);
}

void
Text::draw() {
    if (!mesh || !is_visible) return;
    Mat3 transform = getTransformMatrix();
    model.material = getMaterial();
    renderer->submit(model, transform, layer, render_layer);
}

void
Text::rebuildMesh() {
    MeshData mesh_data;
    mesh_data.vertices.reserve(text.size() * 4);
    mesh_data.indices.reserve(text.size() * 6);

    float offset_x = 0.0f;
    float offset_y = 0.0f;
    uint32_t base = 0;

    for (char c : text) {
        if (c == '\n') {
            offset_x = 0.0;
            offset_y += glyph_h;
            continue;
        }

        GlyphUV uv = font->glyphUv(c);

        mesh_data.vertices.push_back(
            Vertex(offset_x, offset_y, 0.0f, uv.u0, uv.v0, 0.0f)
        );
        mesh_data.vertices.push_back(
            Vertex(offset_x + glyph_w, offset_y, 0.0f, uv.u1, uv.v0, 0.0f)
        );
        mesh_data.vertices.push_back(Vertex(
            offset_x + glyph_w, offset_y + glyph_h, 0.0f, uv.u1, uv.v1, 0.0f
        ));
        mesh_data.vertices.push_back(
            Vertex(offset_x, offset_y + glyph_h, 0.0f, uv.u0, uv.v1, 0.0f)
        );

        mesh_data.indices.push_back(base + 0);
        mesh_data.indices.push_back(base + 1);
        mesh_data.indices.push_back(base + 2);
        mesh_data.indices.push_back(base + 2);
        mesh_data.indices.push_back(base + 3);
        mesh_data.indices.push_back(base + 0);
        base += 4;

        offset_x += advance;
    }

    if (mesh)
        mesh->update(mesh_data);
    else
        mesh = std::make_unique<GPUMesh>(renderer->getDevice(), mesh_data);
    model = Model(mesh.get(), material.get());
}

}  // namespace lili
