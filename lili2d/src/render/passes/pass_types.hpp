#pragma once

#include "render/scene/model.hpp"
#include "geometry/mat3x3.hpp"

namespace lili {

/**
 * \brief Enum defining different rendering layers/passes.
 */
enum class RenderLayer {
	WORLD2D, ///< The 2D world layer.
};

/**
 * \brief A command to draw a model.
 */
struct DrawCommand {
	Model model;    ///< The model to draw.
	Mat3 transform; ///< The transformation matrix.
	float layer;    ///< The layer depth.
};

/**
 * \brief GPU representation of material properties.
 */
struct MaterialGPU {
	float color_tint[4]; ///< The color tint.
};

}  // namespace lili
