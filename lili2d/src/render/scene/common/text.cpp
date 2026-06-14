#include "text.hpp"
#include "../../../geometry/mat3x3.hpp"

namespace lili {

BitmapFont::BitmapFont(
	Renderer *renderer, const std::string &path, uint8_t cols, uint8_t rows
) : cols(cols), rows(rows) {
	texture = std::make_unique<Texture>(renderer->getDevice(), path);
	this->cols = cols;
	this->rows = rows;
	glyph_w = texture->getWidth() / cols;
	glyph_h = texture->getHeight() / rows;
}

Texture *BitmapFont::getTexture() const {
	return texture.get();
}

int BitmapFont::getGlyphW() const {
	return glyph_w;
}

int BitmapFont::getGlyphH() const {
	return glyph_h;
}

GlyphUV BitmapFont::glyphUv(char c) const {
	const int ASCII = static_cast<unsigned char>(c);
	const int FIRST = static_cast<unsigned char>(' ');
	int idx = ASCII - FIRST;
	if (ASCII < FIRST || ASCII > 126)
		idx = '?' - FIRST;
	const int CURRENT_X = idx % cols;
	const int CURRENT_Y = idx / cols;
	const float DELTA_U = 1.0f / static_cast<float>(cols);
	const float DELTA_V = 1.0f / static_cast<float>(rows);

	return (GlyphUV){
		.u0 = CURRENT_X * DELTA_U,
		.v0 = CURRENT_Y * DELTA_V,
		.u1 = (CURRENT_X + 1) * DELTA_U,
		.v1 = (CURRENT_Y + 1) * DELTA_V
	};
}

Text::Text(
	Renderer *renderer, BitmapFont *font, const std::string &text
) {
	this->renderer = renderer;
	this->font = font;
	glyph_w = font->getGlyphW();
	glyph_h = font->getGlyphH();
	advance = glyph_w + 1.0f;
	if (!text.empty())
		this->text = text;
	else
		this->text = "text";
	material = std::make_unique<Material>(font->getTexture());
	material->properties.color_tint = { 1.0f, 1.0f, 1.0f, 1.0f };
	rebuildMesh();
}

void Text::setText(const std::string &value) {
	if (value == text) return;
	text = value;
	rebuildMesh();
}

void Text::setPosition(const Vec2 &position) {
	pos = position;
}

void Text::setSpacing(float value) {
	advance = glyph_w + value;
	rebuildMesh();
}

void Text::setScale(float value) {
	scale = value;
}

void Text::setLayer(float layer) {
	this->layer = layer;
}

void Text::setRender(RenderLayer render_layer) {
	this->render_layer = render_layer;
}

Material* Text::getMaterial() const {
	return material.get();
}

void Text::draw() {
	if (!mesh) return;
	Mat3 translation = Mat3::translate(pos);
	Mat3 scaling = Mat3::scale({ scale, scale });
	Mat3 transform = translation * scaling;
	renderer->submit(model, transform, layer, render_layer);
}

void Text::rebuildMesh() {
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

		mesh_data.vertices.push_back((Vertex){
			.x = offset_x, .y = offset_y, .z = 0.0f,
			.u = uv.u0, .v = uv.v0,
			.material_id = 0
		});
		mesh_data.vertices.push_back((Vertex){
			.x = offset_x + glyph_w, .y = offset_y, .z = 0.0f,
			.u = uv.u1, .v = uv.v0,
			.material_id = 0
		});
		mesh_data.vertices.push_back((Vertex){
			.x = offset_x + glyph_w, .y = offset_y + glyph_h, .z = 0.0f,
			.u = uv.u1, .v = uv.v1,
			.material_id = 0
		});
		mesh_data.vertices.push_back((Vertex){
			.x = offset_x, .y = offset_y + glyph_h, .z = 0.0f,
			.u = uv.u0, .v = uv.v1,
			.material_id = 0
		});

		mesh_data.indices.push_back(base + 0);
		mesh_data.indices.push_back(base + 1);
		mesh_data.indices.push_back(base + 2);
		mesh_data.indices.push_back(base + 2);
		mesh_data.indices.push_back(base + 3);
		mesh_data.indices.push_back(base + 0);
		base += 4;

		offset_x += advance;
	}

	mesh = std::make_unique<GPUMesh>(renderer->getDevice(), mesh_data);
	model = Model(mesh.get(), material.get());
}

}  // namespace lili
