#pragma once

#include "lili2d/core/event.hpp"
#include "lili2d/ecs/ecs_registry.hpp"
#include <memory>
#include <vector>

namespace lili {

class SceneManager;

/// @brief Represents a single game state or screen.
class Scene
{
public:
    /// @brief Constructs a Scene with a name.
    /// @param name Name of the scene.
    explicit Scene(const std::string& name)
      : name(name)
    {
        ecs_registry = std::make_unique<ECSRegistry>();
    }
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
    onEnter()
    {
    }
    /// @brief Called when the scene becomes inactive.
    virtual void
    onExit()
    {
    }
    /// @brief Called when another scene is pushed on top of this one.
    virtual void
    onPause()
    {
    }
    /// @brief Called when a scene on top of this one is popped.
    virtual void
    onResume()
    {
    }

    /// @brief Handles an incoming event.
    /// @param event The event.
    virtual void
    handleEvents([[maybe_unused]] const Event& event)
    {
    }
    /// @brief Updates the scene logic.
    /// @param dt Delta time in seconds used for frame time.
    virtual void
    update([[maybe_unused]] float dt)
    {
    }
    /// @brief Updates the scene physics logic.
    /// @param dt Delta time in seconds used for physics tick.
    virtual void
    fixedUpdate([[maybe_unused]] float dt)
    {
    }
    /// @brief Renders the scene.
    /// @param alpha Interpolation factor (between 0.0 and 1.0).
    virtual void
    render([[maybe_unused]] float alpha)
    {
    }

    /// @brief Sets the parent scene manager.
    /// @param manager The manager.
    inline void
    setManager(SceneManager* manager) noexcept
    {
        this->manager = manager;
    }

    /// @brief Gets the scene name.
    /// @return The name.
    [[nodiscard]] inline const std::string&
    getName() const noexcept
    {
        return name;
    }
    /// @brief Gets the ecs registry of the scene.
    /// @return The ecs registry,
    [[nodiscard]] inline ECSRegistry*
    getECSRegistry() const noexcept
    {
        return ecs_registry.get();
    }
    /// @brief Gets the parent scene manager.
    /// @return The manager.
    [[nodiscard]] inline SceneManager*
    getManager() const noexcept
    {
        return manager;
    }

protected:
    /// @brief Name identifier of the scene.
    std::string name;
    /// @brief The registry for scene's ECS
    std::unique_ptr<ECSRegistry> ecs_registry;

    /// @brief Pointer to managing SceneManager.
    SceneManager* manager = nullptr;
};

/// @brief Manager for transitioning and updating scenes.
class SceneManager
{
public:
    /// @brief Default constructor.
    SceneManager() = default;
    /// @brief Destructor.
    ~SceneManager() = default;

    /// @brief Move constructor.
    SceneManager(SceneManager&&) noexcept = default;
    /// @brief Move assignment operator.
    SceneManager&
    operator=(SceneManager&&) noexcept = default;

    /// @brief Deleted copy constructor.
    SceneManager(const SceneManager&) = delete;
    /// @brief Deleted copy assignment operator.
    SceneManager&
    operator=(const SceneManager&) = delete;

    /// @brief Pushes a new scene onto the stack.
    /// @param scene The scene to push.
    void
    push(std::unique_ptr<Scene> scene);
    /// @brief Pops the top scene from the stack.
    void
    pop();

    /// @brief Replaces the top scene with a new one.
    /// @param scene The new scene.
    void
    changeScene(std::unique_ptr<Scene> scene);

    /// @brief Dispatches an event to the top scene.
    /// @param event The event.
    void
    handleEvents(const Event& event);
    /// @brief Updates the top scene.
    /// @param dt Delta time in seconds.
    void
    update(float dt);
    /// @brief Updates the top scene's physics.
    /// @param dt Delta time in seconds.
    void
    fixedUpdate(float dt);
    /// @brief Renders the top scene.
    /// @param alpha Interpolation factor.
    void
    render(float alpha);

    /// @brief Checks if the scene stack is empty.
    /// @return True if empty, false otherwise.
    [[nodiscard]] inline bool
    empty() const noexcept
    {
        return scenes.empty();
    }

    /// @brief Gets the number of scenes on the stack.
    /// @return Scene count.
    [[nodiscard]] inline size_t
    size() const noexcept
    {
        return scenes.size();
    }

    /// @brief Gets a pointer to the active top scene.
    /// @return Pointer to active Scene, or nullptr if stack is empty.
    [[nodiscard]] inline Scene*
    current() const noexcept
    {
        return scenes.empty() ? nullptr : scenes.back().get();
    }

private:
    std::vector<std::unique_ptr<Scene>> scenes;
};

} // namespace lili
