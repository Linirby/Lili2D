#pragma once

#include <vector>
#include <cassert>
#include "lili2d/ecs/entity.hpp"

namespace lili {

class IComponentPool {
public:
	virtual ~IComponentPool() = default;

	virtual bool has(Entity entity) const = 0;
	virtual void remove(Entity entity) = 0;
};

template<typename T>
class ComponentPool : public IComponentPool {
public:
	template<typename ...Args>
	T &emplace(Entity entity, Args &&...args);
	T &get(Entity entity);
	bool has(Entity entity) const override;
	void remove(Entity entity) override;

	const std::vector<T> &getComponents() const;
	std::vector<T> &getComponents();
	const std::vector<Entity> &getEntities() const;
	size_t size() const;

private:
	std::vector<T> dense_components;
	std::vector<Entity> dense_entities;
	std::vector<size_t> sparse_entities;
	static constexpr size_t EMPTY = static_cast<size_t>(-1);
};

template<typename T>
template<typename ...Args>
T &ComponentPool<T>::emplace(Entity entity, Args &&...args) {
	assert(!has(entity) && "Entity already has this component!");

	uint32_t entity_id = getEntityID(entity);

	dense_components.emplace_back(std::forward<Args>(args)...);
	dense_entities.push_back(entity);
	if (entity_id >= sparse_entities.size())
		sparse_entities.resize(entity_id + 1, EMPTY);

	sparse_entities[entity_id] = dense_components.size() - 1;
	return dense_components.back();
}

template<typename T>
T &ComponentPool<T>::get(Entity entity) {
	assert(has(entity) && "Entity does not have this component!");

	uint32_t entity_id = getEntityID(entity);
	return dense_components[sparse_entities[entity_id]];
}

template<typename T>
bool ComponentPool<T>::has(Entity entity) const {
	uint32_t entity_id = getEntityID(entity);
	return (
		entity_id < sparse_entities.size() &&
		sparse_entities[entity_id] != EMPTY
	);
}

template<typename T>
void ComponentPool<T>::remove(Entity entity) {
	assert(has(entity) && "Cannot remove component: Entity does not have it!");

	uint32_t entity_id = getEntityID(entity);
	size_t idx_to_remove = sparse_entities[entity_id];
	size_t last_dense_idx = dense_components.size() - 1;

	if (idx_to_remove < last_dense_idx) {
		Entity last_entity = dense_entities.back();
		dense_components[idx_to_remove] = std::move(dense_components.back());
		dense_entities[idx_to_remove] = last_entity;
		sparse_entities[getEntityID(last_entity)] = idx_to_remove;
	}

	sparse_entities[entity_id] = EMPTY;
	dense_components.pop_back();
	dense_entities.pop_back();
}

template<typename T>
const std::vector<T> &ComponentPool<T>::getComponents() const {
	return dense_components;
}

template<typename T>
std::vector<T> &ComponentPool<T>::getComponents() {
	return dense_components;
}

template<typename T>
const std::vector<Entity> &ComponentPool<T>::getEntities() const {
	return dense_entities;
}

template<typename T>
size_t ComponentPool<T>::size() const {
	return dense_components.size();
}

}  // namespace lili
