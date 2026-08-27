#pragma once

#include <memory>
#include <vector>

#include "lili2d/scene/scene.hpp"

namespace lili {

/// @brief Manager for transitioning and updating scenes.
class SceneManager {
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
    empty() const noexcept {
        return scenes.empty();
    }

    /// @brief Gets the number of scenes on the stack.
    /// @return Scene count.
    [[nodiscard]] inline size_t
    size() const noexcept {
        return scenes.size();
    }

    /// @brief Gets a pointer to the active top scene.
    /// @return Pointer to active Scene, or nullptr if stack is empty.
    [[nodiscard]] inline Scene*
    current() const noexcept {
        return scenes.empty() ? nullptr : scenes.back().get();
    }

private:
    /// @brief Stack vector of managed scene pointers.
    std::vector<std::unique_ptr<Scene>> scenes;
};

}  // namespace lili
