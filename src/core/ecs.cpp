#include "lili2d/core/ecs.hpp"

namespace lili {

uint32_t getEntityID(Entity entity) {
	return entity & 0x00FFFFFF;
}

uint8_t getEntityGen(Entity entity) {
	return static_cast<uint8_t>(entity >> 24);
}

Entity getEntity(uint32_t id, uint8_t gen) {
	return (static_cast<uint32_t>(gen) << 24) | (id & 0x00FFFFFF);
}

}  // namespace lili
