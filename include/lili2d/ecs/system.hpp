#pragma once

#include <vector>
#include <future>
#include <memory>
#include <algorithm>
#include "lili2d/ecs/entity.hpp"
#include "lili2d/ecs/ecs_registry.hpp"
#include "lili2d/core/thread_pool.hpp"

namespace lili {

/// @brief Non-templated base class for all ECS systems, enabling polymorphic
/// storage.
class SystemBase {
public:
	virtual ~SystemBase() = default;

	/// @brief Runs the system update loop. Decides dynamically between
	/// single-threaded and parallel execution.
	/// @param registry The ECSRegistry.
	/// @param dt Delta time.
	/// @param thread_pool Pointer to the ThreadPool. If nullptr, runs
	// single-threaded.
	virtual void run(
		ECSRegistry& registry, float dt, ThreadPool* thread_pool
	) = 0;
};

/// @brief Templated base class for all ECS systems operating on a specific
/// TargetComponent type.
/// @tparam TargetComponent The component type this system iterates over.
template <typename TargetComponent>
class System : public SystemBase {
public:
	virtual ~System() = default;

	/// @brief The entity count threshold at which multithreading is enabled.
	/// If the component pool size is below this threshold, it executes
	/// sequentially on the main thread.
	size_t parallel_threshold = 1000;

	/// @brief Virtual method that the user overrides to define logic for a
	/// single entity.
	/// @param registry Reference to the ECSRegistry.
	/// @param entity The current entity.
	/// @param component Reference to the target component.
	/// @param dt Delta time.
	virtual void updateEntity(
		ECSRegistry& registry,
		Entity entity,
		TargetComponent& component,
		float dt
	) = 0;

	/// @brief Overridden runner that manages execution strategies.
	void run(ECSRegistry& registry, float dt, ThreadPool* thread_pool) override;

private:
	void runSequentially(
		ECSRegistry& registry,
		const std::vector<Entity>& entities,
		std::vector<TargetComponent>& components,
		float dt
	);

	void runInParallel(
		ECSRegistry& registry,
		const std::vector<Entity>& entities,
		std::vector<TargetComponent>& components,
		float dt,
		ThreadPool& pool
	);
};

template<typename TargetComponent>
void System<TargetComponent>::run(
	ECSRegistry& registry, float dt, ThreadPool* thread_pool
) {
	auto& pool = registry.getPool<TargetComponent>();
	const auto& entities = pool.getEntities();
	auto& components = pool.getComponents();
	size_t total = entities.size();

	if (total == 0)
		return;

	if (
		!thread_pool ||
		thread_pool->getProfile() == PerformanceProfile::YES ||
		total < parallel_threshold
	)
		runSequentially(registry, entities, components, dt);
	else
		runInParallel(registry, entities, components, dt, *thread_pool);
}

template<typename TargetComponent>
void System<TargetComponent>::runSequentially(
	ECSRegistry& registry,
	const std::vector<Entity>& entities,
	std::vector<TargetComponent>& components,
	float dt
) {
	for (size_t i = 0; i < entities.size(); ++i)
		updateEntity(registry, entities[i], components[i], dt);
}

template<typename TargetComponent>
void System<TargetComponent>::runInParallel(
	ECSRegistry& registry,
	const std::vector<Entity>& entities,
	std::vector<TargetComponent>& components,
	float dt,
	ThreadPool& pool
) {
	size_t num_workers = std::thread::hardware_concurrency();
	size_t chunk_size = entities.size() / num_workers;
	if (chunk_size == 0) chunk_size = entities.size();

	std::vector<std::future<void>> futures;

	for (size_t start = 0; start < entities.size(); start += chunk_size) {
		size_t end = std::min(start + chunk_size, entities.size());

		auto promise = std::make_shared<std::promise<void>>();
		futures.push_back(promise->get_future());

		pool.enqueue(
			[
				this,
				&registry,
				&entities,
				&components,
				start,
				end,
				dt,
				promise
			]() {
				for (size_t i = start; i < end; ++i)
					updateEntity(registry, entities[i], components[i], dt);
				promise->set_value();
			}
		);
	}

	for (auto& f : futures)
		f.wait();
}

}  // namespace lili
