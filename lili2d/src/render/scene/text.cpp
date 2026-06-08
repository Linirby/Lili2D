#include "render/scene/text.hpp"
#include "geometry/mat3x3.hpp"

namespace lili {

BitmapFont::BitmapFont(
	Renderer *renderer, const std::string &path, uint8_t cols, uint8_t rows
) {
	texture = std::make_unique<Texture>(renderer->get_device(), path);
	this->cols = cols;
	this->rows = rows;
	glyph_w = texture->get_width() / cols;
	glyph_h = texture->get_height() / rows;
}

Texture *BitmapFont::get_texture() const {
	return texture.get();
}

int BitmapFont::get_glyph_w() const {
	return glyph_w;
}

int BitmapFont::get_glyph_h() const {
	return glyph_h;
}

GlyphUV BitmapFont::glyph_uv(char c) const {
	const int ascii = static_cast<unsigned char>(c);
	const int first = static_cast<unsigned char>(' ');
	int idx = ascii - first;
	if (ascii < first || ascii > 126)
		idx = '?' - first;
	const int current_x = idx % cols;
	const int current_y = idx / cols;
	const float delta_u = 1.0f / static_cast<float>(cols);
	const float delta_v = 1.0f / static_cast<float>(rows);

	return (GlyphUV){
		.u0 = current_x * delta_u,
		.v0 = current_y * delta_v,
		.u1 = (current_x + 1) * delta_u,
		.v1 = (current_y + 1) * delta_v
	};
}

Text::Text(
	Renderer *renderer, BitmapFont *font, const std::string &text
) {
	this->renderer = renderer;
	this->font = font;
	glyph_w = font->get_glyph_w();
	glyph_h = font->get_glyph_h();
	advance = glyph_w + 1.0f;
	if (!text.empty())
		this->text = text;
	else
		this->text = "text";
	material = std::make_unique<Material>(font->get_texture());
	material->properties.color_tint = { 1.0f, 1.0f, 1.0f, 1.0f };
	rebuild_mesh();
}

void Text::set_text(const std::string &value) {
	if (value == text) return;
	text = value;
	rebuild_mesh();
}

void Text::set_position(const Vec2 &position) {
	pos = position;
}

void Text::set_spacing(float value) {
	advance = glyph_w + value;
	rebuild_mesh();
}

void Text::set_scale(float value) {
	scale = value;
}

void Text::set_layer(float layer) {
	this->layer = layer;
}

void Text::set_render(RenderLayer render_layer) {
	this->render_layer = render_layer;
}

Material* Text::get_material() const {
	return material.get();
}

void Text::draw() {
	if (!mesh) return;
	Mat3 translation = Mat3::translate(pos);
	Mat3 scaling = Mat3::scale({ scale, scale });
	Mat3 transform = translation * scaling;
	renderer->submit(model, transform, layer, render_layer);
}

void Text::rebuild_mesh() {
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

		GlyphUV uv = font->glyph_uv(c);

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

	mesh = std::make_unique<GPUMesh>(renderer->get_device(), mesh_data);
	model = Model(mesh.get(), material.get());
}

}  // namespace lili
