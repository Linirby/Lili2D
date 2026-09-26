#pragma once

/// @file render.hpp
/// @brief Aggregates the rendering components of the Lili2D engine, including
/// shapes, sprites, models, and text.

#include "lili2d/render/2d/animated_sprite.hpp"
#include "lili2d/render/2d/circle.hpp"
#include "lili2d/render/2d/line.hpp"
#include "lili2d/render/2d/rect.hpp"
#include "lili2d/render/2d/sprite.hpp"
#include "lili2d/render/2d/sprite_batch.hpp"
#include "lili2d/render/2d/text.hpp"
#include "lili2d/render/2d/ui_layout.hpp"
#include "lili2d/render/common/animation.hpp"
#include "lili2d/render/common/animation_player.hpp"
#include "lili2d/render/common/animation_registry.hpp"
#include "lili2d/render/common/atlas_map.hpp"
#include "lili2d/render/common/camera.hpp"
#include "lili2d/render/common/material.hpp"
#include "lili2d/render/common/material_registry.hpp"
#include "lili2d/render/common/model.hpp"
#include "lili2d/render/common/renderable.hpp"
#include "lili2d/render/common/unit_mesh.hpp"
#include "lili2d/render/gpu/gpu_mesh.hpp"
#include "lili2d/render/gpu/main_graphics_pipeline.hpp"
#include "lili2d/render/gpu/main_render_pass.hpp"
#include "lili2d/render/gpu/pass_types.hpp"
#include "lili2d/render/gpu/shader.hpp"
#include "lili2d/render/gpu/texture.hpp"
#include "lili2d/render/renderer.hpp"
