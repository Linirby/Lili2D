#pragma once

#include "render/scene/model.hpp"
#include "geometry/mat3x3.hpp"

namespace lili {

enum class RenderLayer {
	WORLD2D,
};

struct DrawCommand {
	Model model;
	Mat3 transform;
	float layer;
};

struct MaterialGPU {
	float color_tint[4];
};

}  // namespace lili
