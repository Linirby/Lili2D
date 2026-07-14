#pragma once

#include <vector>
#include <functional>
#include "lili2d/ecs/entity.hpp"
#include "lili2d/ecs/ecs_registry.hpp"

namespace lili {

/// @brief CommandBuffer defers structural ECS modifications (create/destroy,
/// add/remove components) to be executed safely and sequentially on the main
/// thread, avoiding concurrency issues.
class CommandBuffer {
public:
	CommandBuffer() = default;
	~CommandBuffer() = default;

	/// @brief Queue an entity creation command.
	void createEntity();

	/// @brief Queue an entity destruction command.
	/// @param entity The entity to destroy.
	void destroyEntity(Entity entity);

	/// @brief Queue a component emplacement command.
	/// @tparam T The component type.
	/// @tparam Args The constructor arguments for the component.
	/// @param entity The entity.
	/// @param args The arguments to forward to the component constructor.
	template <typename T, typename ...Args>
	void emplaceComponent(Entity entity, Args &&...args);

	/// @brief Queue a component removal command.
	/// @tparam T The component type to remove.
	/// @param entity The entity.
	template <typename T>
	void removeComponent(Entity entity);

	/// @brief Executes all queued commands sequentially on the registry and clears the buffer.
	/// @param registry The ECSRegistry instance to apply commands to.
	void play(ECSRegistry& registry);

	/// @brief Clears all queued commands without executing them.
	void clear();

	// Disable copy/move
	CommandBuffer(const CommandBuffer&) = delete;
	CommandBuffer& operator=(const CommandBuffer&) = delete;
	CommandBuffer(CommandBuffer&&) = delete;
	CommandBuffer& operator=(CommandBuffer&&) = delete;

private:
	std::vector<std::function<void(ECSRegistry&)>> commands;
};

template <typename T, typename ...Args>
void CommandBuffer::emplaceComponent(Entity entity, Args &&...args) {
	commands.push_back(
		[entity, ...fwd_args = std::forward<Args>(args)]
		(ECSRegistry& registry) mutable {
			registry.emplaceComponent<T>(entity, std::move(fwd_args)...);
		}
	);
}

template <typename T>
void CommandBuffer::removeComponent(Entity entity) {
	commands.push_back([entity](ECSRegistry& registry) {
		registry.removeComponent<T>(entity);
	});
}

}  // namespace lili
