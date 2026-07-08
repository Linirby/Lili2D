#pragma once

#include <cstdint>
#include <cassert>
#include <memory>
#include "lili2d/ecs/entity.hpp"
#include "lili2d/ecs/component_pool.hpp"

namespace lili {

/// @brief Central registry that manages entities and their components.
class ECSRegistry {
public:
	/// @brief Checks if the given entity is valid and active.
	/// @param entity The entity to check.
	/// @return True if the entity is valid, false otherwise.
	bool isValid(Entity entity) const;

	/// @brief Creates a new active entity.
	/// @return The created entity.
	Entity createEntity();

	/// @brief Destroys an entity and all its components.
	/// @param entity The entity to destroy.
	void destroyEntity(Entity entity);

	/// @brief Gets the component pool for the specified component type.
	/// @tparam T The component type.
	/// @return Reference to the component pool.
	template<typename T>
	ComponentPool<T> &getPool();

	/// @brief Emplaces a new component for the specified entity.
	/// @tparam T The component type to emplace.
	/// @tparam Args The argument types for constructing the component.
	/// @param entity The entity to add the component to.
	/// @param args The arguments to forward to the component constructor.
	/// @return Reference to the created component.
	template<typename T, typename ...Args>
	T &emplaceComponent(Entity entity, Args &&...args);

	/// @brief Removes a component from the specified entity.
	/// @tparam T The component type to remove.
	/// @param entity The entity.
	template<typename T>
	void removeComponent(Entity entity);

	/// @brief Gets the component of the specified type for the entity.
	/// @tparam T The component type to retrieve.
	/// @param entity The entity.
	/// @return Reference to the component.
	template<typename T>
	T &getComponent(Entity entity);

	/// @brief Checks if the entity has a component of the specified type.
	/// @tparam T The component type to query.
	/// @param entity The entity.
	/// @return True if the component exists, false otherwise.
	template<typename T>
	bool hasComponent(Entity entity) const;

private:
	std::vector<std::unique_ptr<IComponentPool>> component_pools;
	std::vector<Entity> entities;
	std::vector<uint32_t> free_ids;
	static inline uint32_t next_component_type_id = 0;

	/// @brief Gets the unique runtime ID for a component type.
	/// @tparam T The component type.
	/// @return The unique ID.
	template<typename T>
	static uint32_t getComponentTypeID();
};

template<typename T>
ComponentPool<T> &ECSRegistry::getPool() {
	uint32_t type_id = getComponentTypeID<T>();

	if (type_id >= component_pools.size())
		component_pools.resize(type_id + 1);
	if (!component_pools[type_id])
		component_pools[type_id] = std::make_unique<ComponentPool<T>>();

	return *static_cast<ComponentPool<T> *>(component_pools[type_id].get());
}

template<typename T, typename ...Args>
T &ECSRegistry::emplaceComponent(Entity entity, Args &&...args) {
	assert(isValid(entity) && "Cannot add a component from invalid entity");
	return getPool<T>().emplace(entity, std::forward<Args>(args)...);
}

template<typename T>
void ECSRegistry::removeComponent(Entity entity) {
	assert(isValid(entity) && "Cannot remove a component from invalid entity");
	getPool<T>().remove(entity);
}

template<typename T>
T &ECSRegistry::getComponent(Entity entity) {
	assert(isValid(entity) && "Cannot get a component from invalid entity");
	return getPool<T>().get(entity);
}

template<typename T>
bool ECSRegistry::hasComponent(Entity entity) const {
	assert(isValid(entity) && "Cannot query component from invalid entity");
	return const_cast<ECSRegistry *>(this)->getPool<T>().has(entity);
}

template<typename T>
uint32_t ECSRegistry::getComponentTypeID() {
	static uint32_t type_id = next_component_type_id++;
	return type_id;
}

}  // namespace lili
