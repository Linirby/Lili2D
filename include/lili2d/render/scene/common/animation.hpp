#pragma once

#include <vector>
#include "lili2d/render/scene/common/atlas_map.hpp"

namespace lili {

/// @brief Represents an animation as a sequence of frames.
///
/// An Animation is a simple container for a list of AnimationFrame objects.
/// Use AtlasMap to load and slice a spritesheet, then extract Animation
/// objects from it.
class Animation {
public:
	/// @brief Default constructor (empty animation).
	Animation() = default;
	/// @brief Constructs an animation from a pre-built list of frames.
	/// @param frames The sequence of frames.
	Animation(const std::vector<SliceUV> &frames);

	/// @brief Gets the number of frames.
	/// @return The frame count.
	size_t frameCount() const;

	/// @brief Gets a frame by index.
	/// @param index The frame index.
	/// @return Reference to the SliceUV.
	const SliceUV& getFrame(size_t index) const;

private:
	std::vector<SliceUV> frames;
};

}  // namespace lili
