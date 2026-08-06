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

namespace lili {

/// @brief Defines UI anchor alignment points relative to the viewport.
enum class Anchor;
/// @brief A renderable 2D sprite driven by an Animation.
class AnimatedSprite;
/// @brief Represents an animation as a sequence of frames.
class Animation;
/// @brief A player for an animation.
class AnimationPlayer;
/// @brief Registry for managing animations globally.
class AnimationRegistry;
/// @brief Represents an atlas texture map that can be sliced into frames.
class AtlasMap;
/// @brief Represents a bitmap font loaded from an image.
class BitmapFont;
/// @brief A renderable circle.
class Circle;
/// @brief Information structure for a compiled shader bytecode stage.
struct CodeInfo;
/// @brief Creates a unit circle mesh.
/// @param segments The number of segments.
/// @return The mesh data.
MeshData
createUnitCircle(int segments);
/// @brief Creates a unit quad mesh.
/// @return The mesh data.
MeshData
createUnitQuad();
/// @brief Represents a single drawing command within a render pass.
struct DrawCommand;
/// @brief Defines the UV coordinates for a single glyph in a bitmap font.
struct GlyphUV;
/// @brief Represents a mesh allocated on the GPU.
class GPUMesh;
/// @brief Abstract base interface for all renderable 2D objects.
class IRenderable;
/// @brief A renderable line.
class Line;
/// @brief Manages the graphics pipeline state for main 2D rendering.
class MainGraphicsPipeline;
/// @brief Handles rendering operations in the main render pass.
class MainRenderPass;
/// @brief Defines how a model is rendered.
struct Material;
/// @brief GPU-side uniform buffer layout for material properties.
struct MaterialGPU;
/// @brief Properties for a material.
struct MaterialProps;
/// @brief Registry for managing materials globally.
class MaterialRegistry;
/// @brief CPU-side vertex and index container used for mesh generation.
struct MeshData;
/// @brief Associates a GPU mesh with a material for rendering.
struct Model;
/// @brief Defines UI pivot alignment points relative to the element bounding
/// box.
enum class Pivot;
/// @brief A renderable rectangle.
class Rect;
/// @brief Defines render pass target layers for sorting draw commands.
enum class RenderLayer;
/// @brief Main renderer class responsible for handling drawing operations.
class Renderer;
/// @brief Handles loading and management of GPU shader programs.
class Shader;
/// @brief Configuration specifying shader resource bindings.
struct ShaderInfo;
/// @brief Represents a slice of a texture with UV coordinates.
struct SliceUV;
/// @brief Represents a renderable 2D sprite.
class Sprite;
/// @brief Batches multiple sprites into a single draw call.
class SpriteBatch;
/// @brief Represents renderable text using a bitmap font.
class Text;
/// @brief Encapsulates an SDL GPU texture and sampler.
class Texture;
/// @brief Layout configuration for anchoring and positioning UI elements.
struct UILayout;
/// @brief Represents a 2D graphics vertex with position, UV, and color
/// attributes.
struct Vertex;

}  // namespace lili
