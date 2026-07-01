#pragma once

#include <cstdint>
#include <vector>

namespace lili {

typedef uint32_t Entity;

uint32_t getEntityID(Entity entity);
uint8_t getEntityGen(Entity entity);
Entity getEntity(uint32_t id, uint8_t gen);

class IComponentPool {
public:
	virtual ~IComponentPool() = default;

	virtual bool has(Entity entity) const = 0;
	virtual void remove(Entity entity) = 0;
};

template<typename T>
class ComponentPool : IComponentPool {
public:
	template<typename ...Args>
	T &emplace(Entity entity, Args &&...args);
	T &get(Entity entity);
	bool has(Entity entity) const override;
	void remove(Entity entity) override;

private:
	std::vector<T> dense_components;
	std::vector<Entity> dense_entities;
	std::vector<size_t> sparse_entities;
	static constexpr size_t TOMBSTONE = static_cast<size_t>(-1);
};

template<typename T>
template<typename ...Args>
T &ComponentPool<T>::emplace(Entity entity, Args &&...args) {

}

template<typename T>
T &ComponentPool<T>::get(Entity entity) {
	
}

template<typename T>
bool ComponentPool<T>::has(Entity entity) const {
	uint32_t entity_id = getEntityID(entity);
	return (
		entity_id < sparse_entities.size() &&
		sparse_entities[entity_id] != TOMBSTONE
	);
}

template<typename T>
void ComponentPool<T>::remove(Entity entity) {

}

}  // namespace lili
