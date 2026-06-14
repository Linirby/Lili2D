#pragma once

#include <string>
#include <vector>
#include <memory>

#include "../../../geometry/point2.hpp"
#include "../../renderer.hpp"
#include "../../core/texture.hpp"
#include "animation.hpp"

namespace lili {

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

	/// @brief Documentation for getFrame.
	/// @param at_pos The at_pos parameter.
	/// @return The result of getFrame.
	AnimationFrame getFrame(Point2 at_pos) const;
	/// @brief Documentation for getFrame.
	/// @param index The index parameter.
	/// @return The result of getFrame.
	AnimationFrame getFrame(int index) const;
	/// @brief Documentation for getAnimation.
	/// @param start_index The start_index parameter.
	/// @param count The count parameter.
	/// @return The result of getAnimation.
	Animation getAnimation(int start_index, int count) const;
	/// @brief Documentation for getAnimation.
	/// @param start The start parameter.
	/// @param end The end parameter.
	/// @return The result of getAnimation.
	Animation getAnimation(Point2 start, Point2 end) const;

private:
	std::unique_ptr<Texture> full_texture;
	std::vector<AnimationFrame> slices;
	int n_cols = 1, n_rows = 1;
	Point2 unit_size;
};

}  // namespace lili
