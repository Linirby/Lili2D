#pragma once

#include <cstdint>
#include <cassert>
#include <memory>
#include "lili2d/ecs/entity.hpp"
#include "lili2d/ecs/component_pool.hpp"

namespace lili {

class ECSRegistry {
public:
	bool isValid(Entity entity) const;
	Entity createEntity();
	void destroyEntity(Entity entity);

	template<typename T>
	ComponentPool<T> &getPool();
	template<typename T, typename ...Args>
	T &emplaceComponent(Entity entity, Args &&...args);
	template<typename T>
	void removeComponent(Entity entity);
	template<typename T>
	T &getComponent(Entity entity);
	template<typename T>
	bool hasComponent(Entity entity) const;

private:
	std::vector<std::unique_ptr<IComponentPool>> component_pools;
	std::vector<Entity> entities;
	std::vector<uint32_t> free_ids;
	static inline uint32_t next_component_type_id = 0;

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
