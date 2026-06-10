#include "render/scene/atlas_map.hpp"

namespace lili {

AtlasMap::AtlasMap(Renderer *renderer, const std::string &filename) {
	full_texture = std::make_unique<Texture>(renderer->getDevice(), filename);
}

void AtlasMap::slice(int num_columns, int num_rows) {
	n_cols = num_columns;
	n_rows = num_rows;
	unit_size = {
		(int)((float)full_texture->getWidth() / n_cols),
		(int)((float)full_texture->getHeight() / n_rows)
	};

	slices.reserve(num_columns * num_rows);

	float u_step = 1.0f / n_cols;
	float v_step = 1.0f / n_rows;

	for (int j = 0; j < num_rows; ++j) {
		for (int i = 0; i < num_columns; ++i) {
			AnimationFrame slice;
			slice.texture = full_texture.get();
			slice.u_min = i * u_step;
			slice.v_min = j * v_step;
			slice.u_max = (i + 1) * u_step;
			slice.v_max = (j + 1) * v_step;
			slice.width = unit_size.x;
			slice.height = unit_size.y;
			slices.push_back(slice);
		}
	}
}

AnimationFrame AtlasMap::getFrame(Point2 at_pos) const {
	int col = static_cast<int>(at_pos.x);
	int row = static_cast<int>(at_pos.y);
	if (col >= 0 && col < n_cols && row >= 0 && row < n_rows) {
		return getFrame(row * n_cols + col);
	}
	return AnimationFrame();
}

AnimationFrame AtlasMap::getFrame(int index) const {
	if (index >= 0 && (size_t)index < slices.size()) {
		return slices[index];
	}
	return AnimationFrame();
}

Animation AtlasMap::getAnimation(int start_index, int count) const {
	std::vector<AnimationFrame> result;
	result.reserve(count);
	for (int i = 0; i < count; ++i) {
		result.push_back(getFrame(start_index + i));
	}
	return Animation(result);
}

Animation AtlasMap::getAnimation(Point2 start, Point2 end) const {
	int start_col = static_cast<int>(start.x);
	int start_row = static_cast<int>(start.y);
	int end_col = static_cast<int>(end.x);
	int end_row = static_cast<int>(end.y);

	std::vector<AnimationFrame> result;

	if (start_row == end_row) {
		int step = (start_col <= end_col) ? 1 : -1;
		int count = std::abs(end_col - start_col) + 1;
		result.reserve(count);
		for (int i = 0; i < count; ++i) {
			result.push_back(getFrame(Point2(
				start_col + i * step, start_row
			)));
		}
	} else if (start_col == end_col) {
		int step = (start_row <= end_row) ? 1 : -1;
		int count = std::abs(end_row - start_row) + 1;
		result.reserve(count);
		for (int i = 0; i < count; ++i) {
			result.push_back(getFrame(Point2(
				start_col, start_row + i * step
			)));
		}
	} else {
		throw std::runtime_error(
			"AtlasMap::getAnimation: start and end points must be on the "
			"same row or same column."
		);
	}

	return Animation(result);
}

}  // namespace lili
