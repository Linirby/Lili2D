#pragma once

#include <string>
#include <vector>

#include "render/core/texture.hpp"

namespace lili {

class AtlasMap {
public:
	AtlasMap() = default;
	AtlasMap(const std::string &filename);

	~AtlasMap() = default;

	void slice(int num_columns, int num_rows);
	Texture *get_texture(int col, int row) const;
	Texture *get_texture(int index) const;
	std::vector<Texture> *get_textures(
		int start_col, int start_row, int end_col, int end_row
	) const;
	std::vector<Texture> *get_textures(int start_index, int end_index) const;

private:
	std::vector<Texture> textures;
};

}  // namespace lili
