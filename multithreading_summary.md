# Summary: Multithreading in Lili2D

This document provides a concise summary of the multithreading architectures, design patterns, and conflict resolution mechanisms we have discussed, complete with implementation examples.

---

## 1. Modern C++20 Thread Pool
Instead of creating expensive OS threads on-the-fly (`std::async`), we design a persistent **Thread Pool** using modern C++20 primitives:

* **`std::jthread` (Joining Thread):** Automatically requests cooperative shutdown and joins in its destructor, avoiding manual thread lifetime tracking and crashes.
* **`std::stop_token`:** Standardized thread-safe signaling to notify worker threads when it is time to exit.
* **`std::condition_variable_any`:** Sleeps/wakes threads safely, integrating directly with `std::stop_token` to interrupt waiting threads during shutdown.
* **`std::future` / `std::packaged_task`:** Allows submitting arbitrary functions/lambdas and getting the return values back asynchronously.

### Example Thread Pool Code:
```cpp
#include <vector>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <future>
#include <functional>

namespace lili {

class ThreadPool {
public:
    explicit ThreadPool(size_t num_threads = std::thread::hardware_concurrency()) {
        threads.reserve(num_threads);
        for (size_t i = 0; i < num_threads; ++i) {
            threads.emplace_back([this](std::stop_token stop_tok) {
                while (true) {
                    std::function<void()> task;
                    {
                        std::unique_lock<std::mutex> lock(queue_mutex);
                        cv.wait(lock, stop_tok, [this] { return !tasks.empty(); });

                        if (stop_tok.stop_requested() && tasks.empty()) return;

                        task = std::move(tasks.front());
                        tasks.pop();
                    }
                    task();
                }
            });
        }
    }

    ~ThreadPool() { cv.notify_all(); }

    template <typename F, typename... Args>
    auto enqueue(F&& f, Args&&... args) -> std::future<std::invoke_result_t<F, Args...>> {
        using return_type = std::invoke_result_t<F, Args...>;
        auto task = std::make_shared<std::packaged_task<return_type()>>(
            std::bind(std::forward<F>(f), std::forward<Args>(args)...)
        );
        std::future<return_type> result = task->get_future();
        {
            std::unique_lock<std::mutex> lock(queue_mutex);
            tasks.emplace([task]() { (*task)(); });
        }
        cv.notify_one();
        return result;
    }

private:
    std::vector<std::jthread> threads;
    std::queue<std::function<void()>> tasks;
    std::mutex queue_mutex;
    std::condition_variable_any cv;
};

} // namespace lili
```

---

## 2. API Design: "Caller Decides" (Avoiding `...Async` Suffixes)
We avoid duplicating functions into synchronous and asynchronous versions (`foo()` vs `fooAsync()`). 

* **The Rule:** Keep core logic functions strictly synchronous and pure.
* **The Practice:** Let the caller decide where to execute the logic. The caller can run the function directly on the main thread, or wrap it in a lambda and enqueue it to the `ThreadPool`. This makes debugging simple (by toggling a `use_multithreading` flag) and keeps the codebase DRY (Don't Repeat Yourself).

### Example Caller-Decides Toggle:
```cpp
class Chunk {
public:
    // Pure synchronous logic
    ChunkMeshData generateMeshData(Point3 chunk_pos, const Vec2& tile_size) const;

    void rebuild(ThreadPool* thread_pool, bool use_multithreading) {
        if (use_multithreading && thread_pool) {
            // Offload asynchronous execution
            rebuild_future = thread_pool->enqueue([this]() {
                return generateMeshData(pos, size);
            });
        } else {
            // Execute synchronously on the main thread immediately
            ChunkMeshData mesh = generateMeshData(pos, size);
            uploadMeshData(mesh);
        }
    }
};
```

---

## 3. ECS Concurrency Strategies

When parallelizing systems, we have two options:

| Strategy | How it works | Pros | Cons |
| :--- | :--- | :--- | :--- |
| **Data-Parallel (Loops)** | Split a single system's components vector into chunks and process them in parallel using worker threads. | Simple; fits contiguous ECS memory pools. | Main thread still blocks sequentially at the end of each system. |
| **Task-Parallel (DAG)** | Execute entire independent systems (e.g. Physics and AI) concurrently on different threads. | Maximizes multi-core utilization throughout the frame. | Requires dependency tracking and graph scheduling. |

### Example Data-Parallel ECS Loop (Movement System):
```cpp
void updateMovementParallel(
    lili::ECSRegistry &registry, float dt, float w, float h, lili::ThreadPool &pool
) {
    auto &pos_pool = registry.getPool<PositionComponent>();
    const auto &entities = pos_pool.getEntities();
    auto &positions = pos_pool.getComponents();
    size_t total = entities.size();

    size_t num_workers = std::thread::hardware_concurrency();
    size_t chunk_size = total / num_workers;
    if (chunk_size == 0) chunk_size = total;

    std::vector<std::future<void>> futures;

    for (size_t start = 0; start < total; start += chunk_size) {
        size_t end = std::min(start + chunk_size, total);

        futures.push_back(pool.enqueue([&registry, &positions, &entities, start, end, dt, w, h]() {
            for (size_t i = start; i < end; ++i) {
                lili::Entity entity = entities[i];
                if (registry.hasComponent<VelocityComponent>(entity)) {
                    auto &pos = positions[i];
                    auto &vel = registry.getComponent<VelocityComponent>(entity);
                    pos.value += vel.value * dt;
                    // Additional boundary checks...
                }
            }
        }));
    }

    // Wait for all workers to complete processing their assigned chunks
    for (auto &f : futures) f.wait();
}
```

---

## 4. Graph Scheduling & Conflict Resolution
When running multiple ECS systems concurrently, we must avoid data races:

1. **Read-Write / Write-Write Conflicts:**
   - Managed during scheduler compilation.
   - If two systems access the same component type and at least one is writing to it, the scheduler inserts an edge in the DAG, forcing them to run sequentially.
2. **Structural Changes (Creating/Destroying Entities & Components):**
   - Direct concurrent modifications to the registry's sparse sets corrupt memory.
   - **Command Buffer Pattern:** Systems write structural changes into thread-local command buffers. At a frame synchronization point, the main thread plays these command buffers back sequentially, ensuring thread safety with zero lock contention.

### Example Command Buffer (Deferred Changes):
```cpp
#include <vector>
#include <functional>

namespace lili {

class CommandBuffer {
public:
    void destroyEntity(Entity entity) {
        commands.push_back([entity](ECSRegistry& registry) {
            registry.destroyEntity(entity);
        });
    }

    template <typename T, typename... Args>
    void emplaceComponent(Entity entity, Args&&... args) {
        commands.push_back([entity, ...args = std::forward<Args>(args)](ECSRegistry& registry) mutable {
            registry.emplaceComponent<T>(entity, std::move(args)...);
        });
    }

    void play(ECSRegistry& registry) {
        for (const auto& command : commands) {
            command(registry);
        }
        commands.clear();
    }

private:
    std::vector<std::function<void(ECSRegistry&)>> commands;
};

} // namespace lili
```
