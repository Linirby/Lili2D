#pragma once

#include <string>
#include <vector>
#include <memory>

#include "lili2d/geometry/point2.hpp"
#include "lili2d/render/renderer.hpp"
#include "lili2d/render/core/texture.hpp"

namespace lili {

/// @brief Represents a slice of a texture with UV coordinates.
struct SliceUV {
	Texture *texture = nullptr; ///< The texture.
	float u_min = 0.0f; ///< Minimum U coordinate.
	float v_min = 0.0f; ///< Minimum V coordinate.
	float u_max = 1.0f; ///< Maximum U coordinate.
	float v_max = 1.0f; ///< Maximum V coordinate.
	float width = 0.0f; ///< Frame width.
	float height = 0.0f; ///< Frame height.
};

/// @brief Represents AtlasMap.
class AtlasMap {
public:
	AtlasMap() = default;
	/// @brief AtlasMap method.
	AtlasMap(Renderer *renderer, const std::string &filename);
	/// @brief Move constructor.
	AtlasMap(AtlasMap &&) = default;
	/// @brief Move assignment operator.
	/// @return Reference to the assigned sprite.
	AtlasMap &operator=(AtlasMap &&) = default;

	/// @brief Documentation for slice.
	/// @param num_columns The num_columns parameter.
	/// @param num_rows The num_rows parameter.
	void slice(int num_columns, int num_rows);

	/// @brief Documentation for getSliceUV.
	/// @param at_pos The at_pos parameter.
	/// @return The result of getSliceUV.
	SliceUV getSliceUV(Point2 at_pos) const;
	/// @brief Documentation for getSliceUV.
	/// @param index The index parameter.
	/// @return The result of getSliceUV.
	SliceUV getSliceUV(int index) const;
	/// @brief Documentation for getSliceUVs.
	/// @param start_index The start_index parameter.
	/// @param count The count parameter.
	/// @return The result of getSliceUVs.
	std::vector<SliceUV> getSliceUVs(int start_index, int count) const;
	/// @brief Documentation for getSliceUVs.
	/// @param start The start parameter.
	/// @param end The end parameter.
	/// @return The result of getSliceUVs.
	std::vector<SliceUV> getSliceUVs(Point2 start, Point2 end) const;

	Texture *getTexture() const;

private:
	std::unique_ptr<Texture> full_texture;
	std::vector<SliceUV> slices;
	int n_cols = 1, n_rows = 1;
	Point2 unit_size;
};

}  // namespace lili
