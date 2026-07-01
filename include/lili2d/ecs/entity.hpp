#pragma once

#include <cstdint>

namespace lili {

typedef uint32_t Entity;

uint32_t getEntityID(Entity entity);
uint8_t getEntityGen(Entity entity);
Entity getEntity(uint32_t id, uint8_t gen);

}  // namespace lili
