#include "lili2d/ecs/ecs_registry.hpp"

namespace lili {

bool ECSRegistry::isValid(Entity entity) const {
	uint32_t idx = getEntityID(entity);
	return idx < entities.size() && entities[idx] == entity;
}

Entity ECSRegistry::createEntity() {
	if (!free_ids.empty()) {
		uint32_t new_id = free_ids.back();
		free_ids.pop_back();
		return entities[new_id];
	}
	uint32_t new_id = static_cast<uint32_t>(entities.size());
	Entity new_entity = getEntity(new_id, 0);
	entities.push_back(new_entity);
	return new_entity;
}

void ECSRegistry::destroyEntity(Entity entity) {
	if (!isValid(entity))
		return;

	for (std::unique_ptr<IComponentPool> &pool : component_pools)
		if (pool && pool->has(entity))
			pool->remove(entity);

	uint32_t id = getEntityID(entity);
	uint8_t next_gen = getEntityGen(entity) + 1;

	entities[id] = getEntity(id, next_gen);
	free_ids.push_back(id);
}

}  // namespace lili
