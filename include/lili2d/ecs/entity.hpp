#pragma once

#include <cstdint>

namespace lili {

typedef uint32_t Entity;

/// @brief Gets the ID of the entity.
/// @param entity The entity.
/// @return The entity ID.
uint32_t getEntityID(Entity entity);

/// @brief Gets the generation of the entity.
/// @param entity The entity.
/// @return The entity generation.
uint8_t getEntityGen(Entity entity);

/// @brief Creates an entity from an ID and a generation.
/// @param id The entity ID.
/// @param gen The entity generation.
/// @return The created entity.
Entity getEntity(uint32_t id, uint8_t gen);

}  // namespace lili
