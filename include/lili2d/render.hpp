#pragma once

/// @file render.hpp
/// @brief Aggregates the rendering components of the Lili2D engine, including
/// shapes, sprites, models, and text.

#include "lili2d/render/2d/animated_sprite.hpp"  // IWYU pragma: export
#include "lili2d/render/2d/batch_helper.hpp"     // IWYU pragma: export
#include "lili2d/render/2d/bitmap_font.hpp"      // IWYU pragma: export
#include "lili2d/render/2d/circle.hpp"           // IWYU pragma: export
#include "lili2d/render/2d/dynamic_batch.hpp"    // IWYU pragma: export
#include "lili2d/render/2d/line.hpp"             // IWYU pragma: export
#include "lili2d/render/2d/rect.hpp"             // IWYU pragma: export
#include "lili2d/render/2d/render2d_system.hpp"  // IWYU pragma: export
#include "lili2d/render/2d/sprite.hpp"           // IWYU pragma: export
#include "lili2d/render/2d/static_batch.hpp"     // IWYU pragma: export
#include "lili2d/render/2d/text.hpp"             // IWYU pragma: export
#include "lili2d/render/2d/ui_layout.hpp"        // IWYU pragma: export
#include "lili2d/render/2d/ui_layout_system.hpp" // IWYU pragma: export

#include "lili2d/render/common/animation.hpp"          // IWYU pragma: export
#include "lili2d/render/common/animation_player.hpp"   // IWYU pragma: export
#include "lili2d/render/common/animation_registry.hpp" // IWYU pragma: export
#include "lili2d/render/common/atlas_map.hpp"          // IWYU pragma: export
#include "lili2d/render/common/camera.hpp"             // IWYU pragma: export
#include "lili2d/render/common/material.hpp"           // IWYU pragma: export
#include "lili2d/render/common/material_registry.hpp"  // IWYU pragma: export
#include "lili2d/render/common/model.hpp"              // IWYU pragma: export
#include "lili2d/render/common/renderable.hpp"         // IWYU pragma: export
#include "lili2d/render/common/unit_mesh.hpp"          // IWYU pragma: export

#include "lili2d/render/gpu/gpu_mesh.hpp"               // IWYU pragma: export
#include "lili2d/render/gpu/main_graphics_pipeline.hpp" // IWYU pragma: export
#include "lili2d/render/gpu/main_render_pass.hpp"       // IWYU pragma: export
#include "lili2d/render/gpu/pass_types.hpp"             // IWYU pragma: export
#include "lili2d/render/gpu/shader.hpp"                 // IWYU pragma: export
#include "lili2d/render/gpu/texture.hpp"                // IWYU pragma: export

#include "lili2d/render/default_font.hpp"    // IWYU pragma: export
#include "lili2d/render/default_shaders.hpp" // IWYU pragma: export
#include "lili2d/render/renderer.hpp"        // IWYU pragma: export
