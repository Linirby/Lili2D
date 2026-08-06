#pragma once

/// @file lili2d.hpp
/// @brief Main include header for the Lili2D engine. Aggregates all major
/// engine subsystems and forward declarations.

#include "lili2d/core.hpp"
#include "lili2d/ecs.hpp"
#include "lili2d/geometry.hpp"
#include "lili2d/physics.hpp"
#include "lili2d/render.hpp"
#include "lili2d/scene.hpp"
#include "lili2d/world.hpp"

namespace lili {

/// @brief An axis-aligned bounding box 2D for collision detection.
struct AABB2;
/// @brief An axis-aligned bounding box 3D for collision detection.
struct AABB3;
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
/// @brief Centralized facade for engine asset management.
class AssetManager;
/// @brief Generic registry for loading, storing, and referencing game assets.
/// @tparam T The type of the asset.
/// @tparam IdType The type of the ID used to reference the asset.
template <typename T, typename IdType>
class AssetRegistry;
/// @brief Type alias for AssetManager.
using Assets = AssetManager;
/// @brief Represents an atlas texture map that can be sliced into frames.
class AtlasMap;
/// @brief Key used to group tiles by texture and depth for batching.
struct BatchKey;
/// @brief Hash function for BatchKey.
struct BatchKeyHash;
/// @brief Struct holding the batch key and corresponding raw CPU mesh data.
struct BatchMeshData;
/// @brief Represents a bitmap font loaded from an image.
class BitmapFont;
/// @brief Represents a 2D camera for scene viewing.
class Camera;
/// @brief Represents a block of tiles in the world.
struct Chunk;
/// @brief Struct to hold pre-calculated CPU mesh data for all batches of a
/// chunk.
struct ChunkMeshData;
/// @brief A renderable circle.
class Circle;
/// @brief A 2D circle collider for collision detection.
struct CircleCollider;
/// @brief Defines the properties of a 2D circle shape.
struct CircleShape;
/// @brief A utility class for measuring and managing time.
class Clock;
/// @brief Information structure for a compiled shader bytecode stage.
struct CodeInfo;
/// @brief CommandBuffer defers structural ECS modifications.
class CommandBuffer;
/// @brief Contiguous component pool implementation for a specific type T.
/// @tparam T Type of component managed.
template <typename T>
class ComponentPool;
/// @brief Creates a unit circle mesh.
/// @param segments The number of segments.
/// @return The mesh data.
MeshData
createUnitCircle(int segments);
/// @brief Creates a unit quad mesh.
/// @return The mesh data.
MeshData
createUnitQuad();
/// @brief Converts an angle from degrees to radians.
/// @param degrees The angle in degrees.
/// @return The angle in radians.
inline float
degToRad(float degrees);
/// @brief Represents a single drawing command within a render pass.
struct DrawCommand;
/// @brief Manages entities and components.
class ECSRegistry;
/// @brief Engine thread pool configuration settings.
struct EngineConfig;
/// @brief Representation of an entity handle in the ECS.
typedef uint32_t Entity;
/// @brief Base class or container for system events.
class Event;
/// @brief Types of events that can occur in the system.
enum class EventType;
/// @brief Base class for managing the game lifecycle and main loop.
class Game;
/// @brief Central configuration for window and game settings.
class GameConfig;
/// @brief Defines the UV coordinates for a single glyph in a bitmap font.
struct GlyphUV;
/// @brief Represents a mesh allocated on the GPU.
class GPUMesh;
/// @brief Interface for component pools.
class IComponentPool;
/// @brief Abstract base interface for all renderable 2D objects.
class IRenderable;
/// @brief Type-erased base interface for resource managers.
class IResourceManager;
/// @brief Keyboard key actions (e.g., press, release).
enum class KeyAction;
/// @brief Handles keyboard input state and queries.
class Keyboard;
/// @brief Event data for keyboard interactions.
struct KeyboardEvent;
/// @brief A renderable line.
class Line;
/// @brief Defines the properties of a 2D line shape.
struct LineShape;
/// @brief Resource loader function type alias.
template <typename T>
using LoaderFunc = std::function<std::unique_ptr<T>(const std::string&)>;
/// @brief Manages the graphics pipeline state for main 2D rendering.
class MainGraphicsPipeline;
/// @brief Handles rendering operations in the main render pass.
class MainRenderPass;
/// @brief A 3x3 matrix for 2D transformations.
struct Mat3;
/// @brief A 4x4 matrix for 3D or advanced transformations.
struct Mat4;
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
/// @brief Handles mouse input state and queries.
class Mouse;
/// @brief Mouse actions (e.g., press, release, motion).
enum class MouseAction;
/// @brief Mouse buttons (e.g., left, right, middle).
enum class MouseButton;
/// @brief Event data for mouse button interactions.
struct MouseButtonEvent;
/// @brief Event data for mouse motion.
struct MouseMotionEvent;
/// @brief Event data for mouse wheel scrolling.
struct MouseWheelEvent;
/// @brief Performance profiling preset for thread pool worker thread counts.
enum class PerformanceProfile;
/// @brief Defines UI pivot alignment points relative to the element bounding
/// box.
enum class Pivot;
/// @brief An integer-based 2D point.
struct Point2;
/// @brief An integer-based 3D point (chunk indexing).
struct Point3;
/// @brief Comparison functor for Point3, used in std::map.
struct Point3Compare;
/// @brief Converts an angle from radians to degrees.
/// @param radians The angle in radians.
/// @return The angle in degrees.
inline float
radToDeg(float radians);
/// @brief Result data from a raycast operation.
struct RaycastResult;
/// @brief A renderable rectangle.
class Rect;
/// @brief Defines the properties of a 2D rectangle shape.
struct RectShape;
/// @brief Resource reloader function type alias.
template <typename T>
using ReloaderFunc = std::function<bool(T&, const std::string&)>;
/// @brief Main renderer class responsible for handling drawing operations.
class Renderer;
/// @brief Defines render pass target layers for sorting draw commands.
enum class RenderLayer;
/// @brief Generic scoped resource manager with hot-reloading support.
/// @tparam T Type of resource managed.
template <typename T>
class ResourceManager;
/// @brief Record tracking a managed resource and its reloading function.
template <typename T>
struct ResourceRecord;
/// @brief Represents a single game state or screen.
class Scene;
/// @brief Manager for transitioning and updating scenes.
class SceneManager;
/// @brief Custom deleter for SDL_GPUBuffer.
struct SDLGPUBufferDeleter;
/// @brief Custom deleter for SDL_GPUDevice.
struct SDLGPUDeviceDeleter;
/// @brief Custom deleter base for SDL_GPUDevice-dependent objects.
struct SDLGPUDeviceDependentDeleter;
/// @brief Custom deleter for SDL_GPUGraphicsPipeline.
struct SDLGPUGraphicsPipelineDeleter;
/// @brief Custom deleter for SDL_GPUSampler.
struct SDLGPUSamplerDeleter;
/// @brief Custom deleter for SDL_GPUShader.
struct SDLGPUShaderDeleter;
/// @brief Custom deleter for SDL_GPUTexture.
struct SDLGPUTextureDeleter;
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
/// @brief Templated base class for ECS systems operating on specific
/// components.
/// @tparam TargetComponent Component type targeted by the system.
template <typename TargetComponent>
class System;
/// @brief Polymorphic base class for ECS systems.
class SystemBase;
/// @brief Priority levels for enqueued tasks in thread pool.
enum class TaskPriority;
/// @brief Represents renderable text using a bitmap font.
class Text;
/// @brief Encapsulates an SDL GPU texture and sampler.
class Texture;
/// @brief A modern C++20 Thread Pool and Task Scheduler.
class ThreadPool;
/// @brief Represents a single tile in the world.
struct Tile;
/// @brief Manages a grid-based world of chunks and tiles.
class TileMap;
/// @brief Central registry for defining and storing tile types.
class TileRegistry;
/// @brief Layout configuration for anchoring and positioning UI elements.
struct UILayout;
/// @brief A 2D vector.
struct Vec2;
/// @brief A 3D vector.
struct Vec3;
/// @brief A 4D vector.
struct Vec4;
/// @brief Represents a 2D graphics vertex with position, UV, and color
/// attributes.
struct Vertex;
/// @brief Represents an operating system window.
class Window;
/// @brief Event data for window state changes.
struct WindowEvent;
/// @brief Types of window events.
enum class WindowEventType;

}  // namespace lili
