#pragma once

/// @file core.hpp
/// @brief Aggregates the core components of the Lili2D engine, including
/// window management, input handling, and core events.

#include "lili2d/core/action_map.hpp"
#include "lili2d/core/asset_manager.hpp"
#include "lili2d/core/asset_registry.hpp"
#include "lili2d/core/clock.hpp"
#include "lili2d/core/event.hpp"
#include "lili2d/core/game.hpp"
#include "lili2d/core/game_config.hpp"
#include "lili2d/core/keyboard.hpp"
#include "lili2d/core/keys.hpp"
#include "lili2d/core/mouse.hpp"
#include "lili2d/core/resource_manager.hpp"
#include "lili2d/core/sdl_deleters.hpp"
#include "lili2d/core/thread_pool.hpp"
#include "lili2d/core/window.hpp"

namespace lili {

/// @brief Map action with custom set of keys
class ActionMap;
/// @brief Centralized facade for engine asset management.
class AssetManager;
/// @brief Generic registry for loading, storing, and referencing game assets.
/// @tparam T The type of the asset.
/// @tparam IdType The type of the ID used to reference the asset.
template <typename T, typename IdType>
class AssetRegistry;
/// @brief Type alias for AssetManager.
using Assets = AssetManager;
/// @brief A utility class for measuring and managing time.
class Clock;
/// @brief Engine thread pool configuration settings.
struct EngineConfig;
/// @brief Base class or container for system events.
class Event;
/// @brief Types of events that can occur in the system.
enum class EventType;
/// @brief Base class for managing the game lifecycle and main loop.
class Game;
/// @brief Central configuration for window and game settings.
class GameConfig;
/// @brief Type-erased base interface for resource managers.
class IResourceManager;
/// @brief Keyboard scancode enumeration.
enum class Key;
/// @brief Keyboard key actions (e.g., press, release).
enum class KeyAction;
/// @brief Handles keyboard input state and queries.
class Keyboard;
/// @brief Event data for keyboard interactions.
struct KeyboardEvent;
/// @brief Resource loader function type alias.
template <typename T>
using LoaderFunc = std::function<std::unique_ptr<T>(const std::string&)>;
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
/// @brief Resource reloader function type alias.
template <typename T>
using ReloaderFunc = std::function<bool(T&, const std::string&)>;
/// @brief Generic scoped resource manager with hot-reloading support.
/// @tparam T Type of resource managed.
template <typename T>
class ResourceManager;
/// @brief Record tracking a managed resource and its reloading function.
template <typename T>
struct ResourceRecord;
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
/// @brief Priority levels for enqueued tasks in thread pool.
enum class TaskPriority;
/// @brief A modern C++20 Thread Pool and Task Scheduler.
class ThreadPool;
/// @brief Represents an operating system window.
class Window;
/// @brief Event data for window state changes.
struct WindowEvent;
/// @brief Types of window events.
enum class WindowEventType;

}  // namespace lili
