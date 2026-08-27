#pragma once

#include <string>

#include "lili2d/core/event.hpp"
#include "lili2d/render/renderer.hpp"

namespace lili {

/// @brief Manager for transitioning and updating scenes.
class SceneManager;

/// @brief Represents a single game state or screen.
class Scene {
public:
    /// @brief Constructs a Scene with a name and renderer pointer.
    /// @param name Name of the scene.
    /// @param renderer Pointer to Renderer.
    explicit Scene(const std::string& name, Renderer* renderer);
    /// @brief Virtual destructor.
    virtual ~Scene() = default;
    /// @brief Deleted copy constructor.
    Scene(const Scene&) = delete;
    /// @brief Deleted copy assignment operator.
    Scene&
    operator=(const Scene&) = delete;
    /// @brief Deleted move constructor.
    Scene(Scene&&) = delete;
    /// @brief Deleted move assignment operator.
    Scene&
    operator=(Scene&&) = delete;

    /// @brief Called when the scene becomes active.
    virtual void
    onEnter() {}
    /// @brief Called when the scene becomes inactive.
    virtual void
    onExit() {}
    /// @brief Called when another scene is pushed on top of this one.
    virtual void
    onPause() {}
    /// @brief Called when a scene on top of this one is popped.
    virtual void
    onResume() {}

    /// @brief Handles an incoming event.
    /// @param event The event.
    virtual void
    handleEvents(const Event& event);
    /// @brief Updates the scene logic.
    /// @param dt Delta time in seconds.
    virtual void
    update(float dt);
    /// @brief Updates the scene physics logic.
    /// @param dt Delta time in seconds.
    virtual void
    fixedUpdate(float dt);
    /// @brief Renders the scene.
    /// @param alpha Interpolation factor.
    virtual void
    render(float alpha);

    /// @brief Sets the parent scene manager.
    /// @param manager The manager.
    inline void
    setManager(SceneManager* manager) noexcept {
        this->manager = manager;
    }

    /// @brief Gets the scene name.
    /// @return The name.
    [[nodiscard]] inline const std::string&
    getName() const noexcept {
        return name;
    }
    /// @brief Gets the parent scene manager.
    /// @return The manager.
    [[nodiscard]] inline SceneManager*
    getManager() const noexcept {
        return manager;
    }
    /// @brief Gets the renderer.
    /// @return Pointer to Renderer.
    [[nodiscard]] inline Renderer*
    getRenderer() const noexcept {
        return renderer;
    }

protected:
    /// @brief Name identifier of the scene.
    std::string name;
    /// @brief Pointer to Renderer instance.
    Renderer* renderer = nullptr;
    /// @brief Pointer to managing SceneManager.
    SceneManager* manager = nullptr;
};

}  // namespace lili
