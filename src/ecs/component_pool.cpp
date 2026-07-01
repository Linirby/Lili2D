#include "lili2d/ecs/component_pool.hpp"

namespace lili {

template<typename T>
template<typename ...Args>
T &ComponentPool<T>::emplace(Entity entity, Args &&...args) {
	assert(!has(entity) && "Entity already has this component!");

	uint32_t entity_id = getEntityID(entity);

	dense_components.emplace_back(std::forward<Args>(args)...);
	dense_entities.push_back(entity);
	if (entity_id >= sparse_entities.size())
		sparse_entities.resize(entity_id + 1, TOMBSTONE);

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
		sparse_entities[entity_id] != TOMBSTONE
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

	sparse_entities[entity_id] = TOMBSTONE;
	dense_components.pop_back();
	dense_entities.pop_back();
}

}  // namespace lili
