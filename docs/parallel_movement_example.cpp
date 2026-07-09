#include <vector>
#include <future>
#include <iostream>
#include "lili2d/ecs.hpp"
#include "lili2d/core/thread_pool.hpp" // Assumes the thread pool we designed is placed here

// Define the components used in the example
struct PositionComponent {
    lili::Vec2 value;
};

struct VelocityComponent {
    lili::Vec2 value;
};

struct RenderComponent {
    float radius;
};

namespace systems {

/**
 * ❌ ORIGINAL SINGLE-THREADED ECS SYSTEM
 * Updates positions sequentially on the main thread.
 */
void updateMovementSingleThreaded(
    lili::ECSRegistry &registry, float dt, float window_w, float window_h
) {
    auto &pos_pool = registry.getPool<PositionComponent>();
    const auto &entities = pos_pool.getEntities();
    auto &positions = pos_pool.getComponents();

    for (size_t i = 0; i < entities.size(); ++i) {
        lili::Entity entity = entities[i];
        if (registry.hasComponent<VelocityComponent>(entity)) {
            auto &pos = positions[i];
            auto &vel = registry.getComponent<VelocityComponent>(entity);

            pos.value += vel.value * dt;

            if (registry.hasComponent<RenderComponent>(entity)) {
                auto &render = registry.getComponent<RenderComponent>(entity);

                if (pos.value.x - render.radius < 0.0f) {
                    pos.value.x = render.radius;
                    vel.value.x = -vel.value.x;
                } else if (pos.value.x + render.radius > window_w) {
                    pos.value.x = window_w - render.radius;
                    vel.value.x = -vel.value.x;
                }

                if (pos.value.y - render.radius < 0.0f) {
                    pos.value.y = render.radius;
                    vel.value.y = -vel.value.y;
                } else if (pos.value.y + render.radius > window_h) {
                    pos.value.y = window_h - render.radius;
                    vel.value.y = -vel.value.y;
                }
            }
        }
    }
}

/**
 * ✅ PARALLELIZED ECS SYSTEM USING THE THREAD POOL
 * Divides the entities into chunks and processes them across worker threads.
 */
void updateMovementParallel(
    lili::ECSRegistry &registry, float dt, float window_w, float window_h, lili::ThreadPool &pool
) {
    auto &pos_pool = registry.getPool<PositionComponent>();
    const auto &entities = pos_pool.getEntities();
    auto &positions = pos_pool.getComponents();
    size_t total_entities = entities.size();

    if (total_entities == 0) return;

    // 1. Determine batch sizes based on workload.
    // If you have only a few entities, multithreading adds overhead. Only use it for larger loads.
    constexpr size_t MIN_BATCH_SIZE = 100;
    if (total_entities < MIN_BATCH_SIZE) {
        updateMovementSingleThreaded(registry, dt, window_w, window_h);
        return;
    }

    // 2. Divide work into chunks based on available thread workers
    size_t num_workers = std::thread::hardware_concurrency();
    size_t chunk_size = total_entities / num_workers;
    if (chunk_size == 0) chunk_size = total_entities;

    std::vector<std::future<void>> futures;

    for (size_t start_idx = 0; start_idx < total_entities; start_idx += chunk_size) {
        size_t end_idx = std::min(start_idx + chunk_size, total_entities);

        // 3. Enqueue the workload chunk to the thread pool
        futures.push_back(pool.enqueue([&registry, &positions, &entities, start_idx, end_idx, dt, window_w, window_h]() {
            for (size_t i = start_idx; i < end_idx; ++i) {
                lili::Entity entity = entities[i];
                
                // Safe read access: Multiple threads querying different entities is safe.
                // Mutating registry structures (adding/removing components) is NOT safe here.
                if (registry.hasComponent<VelocityComponent>(entity)) {
                    auto &pos = positions[i];
                    auto &vel = registry.getComponent<VelocityComponent>(entity);

                    pos.value += vel.value * dt;

                    if (registry.hasComponent<RenderComponent>(entity)) {
                        auto &render = registry.getComponent<RenderComponent>(entity);

                        if (pos.value.x - render.radius < 0.0f) {
                            pos.value.x = render.radius;
                            vel.value.x = -vel.value.x;
                        } else if (pos.value.x + render.radius > window_w) {
                            pos.value.x = window_w - render.radius;
                            vel.value.x = -vel.value.x;
                        }

                        if (pos.value.y - render.radius < 0.0f) {
                            pos.value.y = render.radius;
                            vel.value.y = -vel.value.y;
                        } else if (pos.value.y + render.radius > window_h) {
                            pos.value.y = window_h - render.radius;
                            vel.value.y = -vel.value.y;
                        }
                    }
                }
            }
        }));
    }

    // 4. Synchronize: Wait for all threads to finish processing their chunk
    // this keeps onUpdate synchronous from the game loop's perspective.
    for (auto &f : futures) {
        f.wait();
    }
}

} // namespace systems
