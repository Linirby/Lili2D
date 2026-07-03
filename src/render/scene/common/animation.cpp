#include "lili2d/render/scene/common/animation.hpp"

namespace lili {

Animation::Animation(const std::vector<SliceUV> &frames)
	: frames(frames) {}

size_t Animation::frameCount() const {
	return frames.size();
}

const SliceUV& Animation::getFrame(size_t index) const {
	return frames[index];
}

}  // namespace lili
