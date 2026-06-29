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

/// @brief Represents an atlas texture map that can be sliced into frames.
class AtlasMap {
public:
	AtlasMap() = default;
	/// @brief Constructs an AtlasMap and loads its texture from a file.
	/// @param renderer The renderer used to allocate the texture.
	/// @param filename The path to the image file.
	AtlasMap(Renderer *renderer, const std::string &filename);
	/// @brief Move constructor.
	AtlasMap(AtlasMap &&) = default;
	/// @brief Move assignment operator.
	/// @return Reference to the assigned atlas map.
	AtlasMap &operator=(AtlasMap &&) = default;

	/// @brief Slices the underlying texture into a grid of uniform frames.
	/// @param num_columns The number of columns in the sprite sheet.
	/// @param num_rows The number of rows in the sprite sheet.
	void slice(int num_columns, int num_rows);

	/// @brief Gets the SliceUV for a specific column and row.
	/// @param at_pos The 2D position (col, row) of the frame.
	/// @return The SliceUV at the specified position.
	SliceUV getSliceUV(Point2 at_pos) const;
	/// @brief Gets the SliceUV using a linear 1D index (left-to-right, down).
	/// @param index The 0-based linear index of the frame.
	/// @return The SliceUV at the specified index.
	SliceUV getSliceUV(int index) const;
	/// @brief Gets a contiguous sequence of SliceUVs using a linear 1D index.
	/// @param start_index The 0-based linear index to start from.
	/// @param count The number of frames to retrieve.
	/// @return A vector of SliceUVs.
	std::vector<SliceUV> getSliceUVs(int start_index, int count) const;
	/// @brief Gets a contiguous sequence of SliceUVs along a row or column.
	/// @param start The 2D starting position (col, row).
	/// @param end The 2D ending position (col, row). Must be on the same axis.
	/// @return A vector of SliceUVs.
	std::vector<SliceUV> getSliceUVs(Point2 start, Point2 end) const;

	/// @brief Gets the underlying Texture object.
	/// @return Pointer to the Texture.
	Texture *getTexture() const;

private:
	std::unique_ptr<Texture> full_texture;
	std::vector<SliceUV> slices;
	int n_cols = 1, n_rows = 1;
	Point2 unit_size;
};

}  // namespace lili
