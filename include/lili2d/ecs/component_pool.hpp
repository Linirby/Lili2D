#pragma once

#include <cassert>
#include <cstddef>
#include <vector>

#include "lili2d/ecs/entity.hpp"

namespace lili {

/// @brief Interface for a type-erased component pool.
class IComponentPool {
public:
    /// @brief Default constructor.
    IComponentPool() = default;
    /// @brief Virtual destructor.
    virtual ~IComponentPool() = default;
    /// @brief Deleted copy constructor.
    IComponentPool(const IComponentPool&) = delete;
    /// @brief Deleted copy assignment operator.
    IComponentPool&
    operator=(const IComponentPool&) = delete;
    /// @brief Deleted move constructor.
    IComponentPool(IComponentPool&&) = delete;
    /// @brief Deleted move assignment operator.
    IComponentPool&
    operator=(IComponentPool&&) = delete;

    /// @brief Checks if the entity has a component in this pool.
    /// @param entity The entity to check.
    /// @return True if the component exists, false otherwise.
    [[nodiscard]] virtual bool
    has(Entity entity) const noexcept = 0;

    /// @brief Removes the component for the entity from this pool.
    /// @param entity The entity.
    virtual void
    remove(Entity entity) = 0;

    /// @brief Gets the number of active components in this pool.
    /// @return The number of components.
    [[nodiscard]] virtual size_t
    size() const noexcept = 0;

    /// @brief Get the entity at a specific index in this pool.
    /// @param index The index where the entity is located.
    /// @return The entity at the corresponding index.
    [[nodiscard]] virtual Entity
    getEntity(size_t index) const = 0;

    /// @brief Check if the pool is empty.
    /// @return True if empty, False if not.
    [[nodiscard]] virtual bool
    empty() const noexcept = 0;

    /// @brief Gets a const reference to the vector of entities in this pool.
    /// @return Const reference to the entities vector.
    [[nodiscard]] virtual const std::vector<Entity>&
    getEntities() const noexcept = 0;
};

/// @brief Contiguous component pool implementation for a specific type T.
/// @tparam T The component type.
template <typename T>
class ComponentPool : public IComponentPool {
public:
    /// @brief Emplaces a new component for the entity.
    /// @tparam Args The argument types for constructing the component.
    /// @param entity The entity.
    /// @param args The arguments to forward to the component constructor.
    /// @return Reference to the created component.
    template <typename... Args>
    T&
    emplace(Entity entity, Args&&... args) {
        assert(!has(entity) && "Entity already has this component!");

        uint32_t entity_id = getEntityID(entity);

        dense_components.emplace_back(std::forward<Args>(args)...);
        dense_entities.push_back(entity);
        if (entity_id >= sparse_entities.size())
            sparse_entities.resize(entity_id + 1, EMPTY);

        sparse_entities[entity_id] = dense_components.size() - 1;
        return dense_components.back();
    }

    /// @brief Gets the component for the entity.
    /// @param entity The entity.
    /// @return Reference to the component.
    [[nodiscard]] T&
    get(Entity entity) noexcept {
        assert(has(entity) && "Entity does not have this component!");

        uint32_t entity_id = getEntityID(entity);
        return dense_components[sparse_entities[entity_id]];
    }

    /// @brief Gets a const reference to the component for the entity.
    /// @param entity The entity.
    /// @return Const reference to the component.
    [[nodiscard]] const T&
    get(Entity entity) const noexcept {
        assert(has(entity) && "Entity does not have this component!");

        uint32_t entity_id = getEntityID(entity);
        return dense_components[sparse_entities[entity_id]];
    }

    /// @brief Checks if the entity has a component in this pool.
    /// @param entity The entity to check.
    /// @return True if the component exists, false otherwise.
    [[nodiscard]] bool
    has(Entity entity) const noexcept override {
        uint32_t entity_id = getEntityID(entity);
        return (
            entity_id < sparse_entities.size() &&
            sparse_entities[entity_id] != EMPTY
        );
    }

    /// @brief Removes the component for the entity from this pool.
    /// @param entity The entity.
    void
    remove(Entity entity) override {
        assert(
            has(entity) && "Cannot remove component: Entity does not have it!"
        );

        uint32_t entity_id = getEntityID(entity);
        size_t idx_to_remove = sparse_entities[entity_id];
        size_t last_dense_idx = dense_components.size() - 1;

        if (idx_to_remove < last_dense_idx) {
            Entity last_entity = dense_entities.back();
            dense_components[idx_to_remove] =
                std::move(dense_components.back());
            dense_entities[idx_to_remove] = last_entity;
            sparse_entities[getEntityID(last_entity)] = idx_to_remove;
        }

        sparse_entities[entity_id] = EMPTY;
        dense_components.pop_back();
        dense_entities.pop_back();
    }

    /// @brief Gets the number of active components in this pool.
    /// @return The number of components.
    [[nodiscard]] size_t
    size() const noexcept override {
        return dense_components.size();
    }

    /// @brief Get the entity at a specific index in this pool.
    /// @param index The index where the entity is located.
    /// @return The entity at the corresponding index.
    [[nodiscard]] virtual Entity
    getEntity(size_t index) const override {
        return dense_entities[index];
    }

    /// @brief Check if the pool is empty.
    /// @return True if empty, False if not.lead_pool->size()
    [[nodiscard]] virtual bool
    empty() const noexcept override {
        return dense_components.empty();
    }

    /// @brief Gets a const reference to the vector of components.
    /// @return Const reference to the components vector.
    [[nodiscard]] const std::vector<T>&
    getComponents() const noexcept {
        return dense_components;
    }

    /// @brief Gets a mutable reference to the vector of components.
    /// @return Mutable reference to the components vector.
    [[nodiscard]] std::vector<T>&
    getComponents() noexcept {
        return dense_components;
    }

    /// @brief Gets a const reference to the vector of entities in this pool.
    /// @return Const reference to the entities vector.
    [[nodiscard]] const std::vector<Entity>&
    getEntities() const noexcept override {
        return dense_entities;
    }

private:
    std::vector<T> dense_components;
    std::vector<Entity> dense_entities;
    std::vector<size_t> sparse_entities;
    static constexpr size_t EMPTY = static_cast<size_t>(-1);
};

}  // namespace lili
