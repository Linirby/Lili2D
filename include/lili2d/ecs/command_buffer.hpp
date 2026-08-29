#pragma once

#include <functional>
#include <vector>

#include "lili2d/ecs/ecs_registry.hpp"
#include "lili2d/ecs/entity.hpp"

namespace lili {

/// @brief CommandBuffer defers structural ECS modifications (create/destroy,
/// add/remove components) to be executed safely and sequentially on the main
/// thread, avoiding concurrency issues.
class CommandBuffer {
public:
    /// @brief Default constructor.
    CommandBuffer() = default;
    /// @brief Default destructor.
    ~CommandBuffer() = default;

    /// @brief Move constructor.
    CommandBuffer(CommandBuffer&&) noexcept = default;
    /// @brief Move assignment operator.
    CommandBuffer&
    operator=(CommandBuffer&&) noexcept = default;

    /// @brief Deleted copy constructor.
    CommandBuffer(const CommandBuffer&) = delete;
    /// @brief Deleted copy assignment operator.
    CommandBuffer&
    operator=(const CommandBuffer&) = delete;

    /// @brief Queue an entity creation command.
    void
    createEntity();

    /// @brief Queue an entity destruction command.
    /// @param entity The entity to destroy.
    void
    destroyEntity(Entity entity);

    /// @brief Queue a component emplacement command.
    /// @tparam T The component type.
    /// @tparam Args The constructor arguments for the component.
    /// @param entity The entity.
    /// @param args The arguments to forward to the component constructor.
    template <typename T, typename... Args>
    void
    emplaceComponent(Entity entity, Args&&... args);

    /// @brief Queue a component removal command.
    /// @tparam T The component type to remove.
    /// @param entity The entity.
    template <typename T>
    void
    removeComponent(Entity entity);

    /// @brief Executes all queued commands sequentially on the registry and
    /// clears the buffer.
    /// @param registry The ECSRegistry instance to apply commands to.
    void
    play(ECSRegistry& registry);

    /// @brief Clears all queued commands without executing them.
    void
    clear() noexcept;

    /// @brief Checks if there are no queued commands.
    /// @return True if empty, false otherwise.
    [[nodiscard]] inline bool
    empty() const noexcept {
        return commands.empty();
    }

    /// @brief Gets the number of queued commands.
    /// @return Number of commands.
    [[nodiscard]] inline size_t
    size() const noexcept {
        return commands.size();
    }

private:
    std::vector<std::function<void(ECSRegistry&)>> commands;
};

template <typename T, typename... Args>
void
CommandBuffer::emplaceComponent(Entity entity, Args&&... args) {
    commands.push_back([entity, ... fwd_args = std::forward<Args>(args)](
                           ECSRegistry& registry
                       ) mutable {
        registry.emplaceComponent<T>(entity, std::move(fwd_args)...);
    });
}

template <typename T>
void
CommandBuffer::removeComponent(Entity entity) {
    commands.push_back([entity](ECSRegistry& registry) {
        registry.removeComponent<T>(entity);
    });
}

}  // namespace lili
