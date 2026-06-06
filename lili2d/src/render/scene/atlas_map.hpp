#pragma once

#include <string>
#include <vector>
#include <memory>

#include "geometry/point2.hpp"
#include "render/renderer.hpp"
#include "render/core/texture.hpp"
#include "render/scene/animation.hpp"

namespace lili {

class AtlasMap {
public:
	AtlasMap() = default;
	AtlasMap(Renderer *renderer, const std::string &filename);

	/// \brief Move constructor.
	AtlasMap(AtlasMap &&) = default;
	/**
	 * \brief Move assignment operator.
	 * \return Reference to the assigned sprite.
	 */
	AtlasMap &operator=(AtlasMap &&) = default;

	~AtlasMap() = default;

	void slice(int num_columns, int num_rows);
	
	AnimationFrame get_frame(Point2 at_pos) const;
	AnimationFrame get_frame(int index) const;

	Animation get_animation(int start_index, int count) const;
	Animation get_animation(Point2 start, Point2 end) const;

private:
	std::unique_ptr<Texture> full_texture;
	std::vector<AnimationFrame> slices;
	int n_cols = 1, n_rows = 1;
	Point2 unit_size;
};

}  // namespace lili
