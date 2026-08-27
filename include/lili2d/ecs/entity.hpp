#pragma once

#include <cstdint>

namespace lili {

/// @brief Representation of an entity handle encoding ID and generation.
typedef uint32_t Entity;

/// @brief Gets the ID of the entity.
/// @param entity The entity.
/// @return The entity ID.
[[nodiscard]] constexpr inline uint32_t
getEntityID(Entity entity) noexcept {
    return entity & 0x00FFFFFF;
}

/// @brief Gets the generation of the entity.
/// @param entity The entity.
/// @return The entity generation.
[[nodiscard]] constexpr inline uint8_t
getEntityGen(Entity entity) noexcept {
    return static_cast<uint8_t>(entity >> 24);
}

/// @brief Creates an entity from an ID and a generation.
/// @param id The entity ID.
/// @param gen The entity generation.
/// @return The created entity.
[[nodiscard]] constexpr inline Entity
getEntity(uint32_t id, uint8_t gen) noexcept {
    return (static_cast<uint32_t>(gen) << 24) | (id & 0x00FFFFFF);
}

}  // namespace lili
