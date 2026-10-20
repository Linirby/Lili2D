#pragma once

#include <algorithm>
#include <cstddef>
#include <future>
#include <memory>
#include <tuple>
#include <vector>

#include "lili2d/core/thread_pool.hpp"
#include "lili2d/ecs/component_pool.hpp"
#include "lili2d/ecs/ecs_registry.hpp"
#include "lili2d/ecs/ecs_view.hpp"
#include "lili2d/ecs/entity.hpp"

namespace lili {

/// @brief Non-templated base class for all ECS systems, enabling polymorphic
/// storage.
class SystemBase
{
public:
    /// @brief Default virtual destructor.
    virtual ~SystemBase() = default;

    /// @brief Runs the system update loop. Decides dynamically between
    /// single-threaded and parallel execution.
    /// @param registry The ECSRegistry.
    /// @param dt Delta time.
    /// @param thread_pool Pointer to the ThreadPool. If nullptr, runs
    /// single-threaded.
    virtual void
    run(ECSRegistry& registry, float dt, ThreadPool* thread_pool) = 0;
};

/// @brief Templated base class for all ECS systems operating on a specific
/// set of component types.
/// @tparam Components The component types this system iterates over.
template<typename... Components>
class System : public SystemBase
{
public:
    /// @brief Virtual destructor.
    virtual ~System() = default;

    /// @brief The entity count threshold at which multithreading is enabled.
    /// If the component pool size is below this threshold, it executes
    /// sequentially on the main thread.
    size_t parallel_threshold = 1000;

    /// @brief Virtual method that the user overrides to define logic for a
    /// single entity.
    /// @param registry Reference to the ECSRegistry.
    /// @param entity The current entity.
    /// @param component References to the target components.
    /// @param dt Delta time.
    virtual void
    updateEntity(
        ECSRegistry& registry,
        Entity entity,
        Components&... component,
        float dt
    ) = 0;

    /// @brief Overridden runner that manages execution strategies via ECSView.
    void
    run(ECSRegistry& registry, float dt, ThreadPool* thread_pool) override
    {
        registry.view<Components...>().parallelEach(
            thread_pool,
            [this, &registry, dt](Entity entity, Components&... comps) {
                updateEntity(registry, entity, comps..., dt);
            },
            parallel_threshold
        );
    }
};

} // namespace lili
