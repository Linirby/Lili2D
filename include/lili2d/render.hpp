#pragma once

/// @file render.hpp
/// @brief Aggregates the rendering components of the Lili2D engine, including
/// shapes, sprites, models, and text.

#include "lili2d/render/core/gpu_mesh.hpp"
#include "lili2d/render/core/shader.hpp"
#include "lili2d/render/core/texture.hpp"
#include "lili2d/render/interfaces/renderable.hpp"
#include "lili2d/render/passes/main_render_pass.hpp"
#include "lili2d/render/passes/pass_types.hpp"
#include "lili2d/render/pipelines/main_graphics_pipeline.hpp"
#include "lili2d/render/renderer.hpp"
#include "lili2d/render/scene/common/animation.hpp"
#include "lili2d/render/scene/common/animation_player.hpp"
#include "lili2d/render/scene/common/animation_registry.hpp"
#include "lili2d/render/scene/common/atlas_map.hpp"
#include "lili2d/render/scene/common/material.hpp"
#include "lili2d/render/scene/common/material_registry.hpp"
#include "lili2d/render/scene/common/model.hpp"
#include "lili2d/render/scene/common/text.hpp"
#include "lili2d/render/scene/common/utils.hpp"
#include "lili2d/render/scene/core2d/animated_sprite.hpp"
#include "lili2d/render/scene/core2d/sprite.hpp"
#include "lili2d/render/scene/core2d/sprite_batch.hpp"
#include "lili2d/render/scene/shapes/circle.hpp"
#include "lili2d/render/scene/shapes/line.hpp"
#include "lili2d/render/scene/shapes/rect.hpp"
#include "lili2d/render/ui/ui_layout.hpp"
#include "lili2d/render/white_1x1_png.hpp"
