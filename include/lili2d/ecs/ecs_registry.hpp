#pragma once

#include <cstdint>
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
	static inline uint32_t next_component_type_id = 0;

	template<typename T>
	static uint32_t getComponentTypeID();
};

bool ECSRegistry::isValid(Entity entity) const {}

Entity ECSRegistry::createEntity() {}

void ECSRegistry::destroyEntity(Entity entity) {}

template<typename T>
ComponentPool<T> &ECSRegistry::getPool() {}

template<typename T, typename ...Args>
T &ECSRegistry::emplaceComponent(Entity entity, Args &&...args) {}

template<typename T>
void ECSRegistry::removeComponent(Entity entity) {}

template<typename T>
T &ECSRegistry::getComponent(Entity entity) {}

template<typename T>
bool ECSRegistry::hasComponent(Entity entity) const {}

template<typename T>
uint32_t ECSRegistry::getComponentTypeID() {
	static uint32_t type_id = next_component_type_id++;
	return type_id;
}

}  // namespace lili
