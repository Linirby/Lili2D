# Guide: Modern C++20 Thread Pool & Task Scheduler

Multi-threaded programming can feel intimidating, but C++20 introduces features like `std::jthread` and `std::stop_token` that make building a safe, clean, and robust **Thread Pool** much simpler than in the past.

Here is a step-by-step guide to building a modern thread pool and integrating it into your engine.

---

## 1. The Core Concepts

A Thread Pool consists of three main parts:
1. **Worker Threads:** A fixed set of background threads that run in an infinite loop, waiting for tasks to do.
2. **Task Queue:** A FIFO (First-In, First-Out) queue containing the functions/tasks that need execution.
3. **Synchronization (Mutex & Condition Variable):** Mechanisms to prevent data races when multiple threads access the queue, and to sleep/wake up threads so they do not consume 100% CPU while waiting.

---

## 2. The Implementation (C++20)

Save this as a header (e.g., `include/lili2d/core/thread_pool.hpp`):

```cpp
#pragma once

#include <vector>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <future>
#include <functional>
#include <concepts>

namespace lili {

class ThreadPool {
public:
    // 1. Constructor: Spawns worker threads
    explicit ThreadPool(size_t num_threads = std::thread::hardware_concurrency()) {
        threads.reserve(num_threads);
        for (size_t i = 0; i < num_threads; ++i) {
            threads.emplace_back([this](std::stop_token stop_tok) {
                worker_loop(stop_tok);
            });
        }
    }

    // 2. Destructor: Automatically joins threads thanks to std::jthread
    ~ThreadPool() {
        // We notify all threads so they wake up and check stop_token
        cv.notify_all();
    }

    // 3. Submit a task and return a std::future so the caller can wait for the result
    template <typename F, typename... Args>
    auto enqueue(F&& f, Args&&... args) -> std::future<std::invoke_result_t<F, Args...>> {
        using return_type = std::invoke_result_t<F, Args...>;

        // Package the function and its arguments into a task
        auto task = std::make_shared<std::packaged_task<return_type()>>(
            std::bind(std::forward<F>(f), std::forward<Args>(args)...)
        );
        
        std::future<return_type> result = task->get_future();
        {
            std::unique_lock<std::mutex> lock(queue_mutex);
            
            // Type-erase the task into a void() function and push it
            tasks.emplace([task]() { (*task)(); });
        }
        
        // Wake up one worker thread
        cv.notify_one();
        return result;
    }

    // Disable copying
    ThreadPool(const ThreadPool&) = delete;
    ThreadPool& operator=(const ThreadPool&) = delete;

private:
    void worker_loop(std::stop_token stop_tok) {
        while (true) {
            std::function<void()> task;
            {
                std::unique_lock<std::mutex> lock(queue_mutex);
                
                // C++20 cv.wait overloaded for stop_token. 
                // It wakes up if:
                //   1) A new task is added (!tasks.empty())
                //   2) The thread pool is shutting down (stop_tok.stop_requested())
                cv.wait(lock, stop_tok, [this] {
                    return !tasks.empty();
                });

                // Exit the loop if stop is requested and queue is empty
                if (stop_tok.stop_requested() && tasks.empty()) {
                    return;
                }

                // Grab the next task
                task = std::move(tasks.front());
                tasks.pop();
            }

            // Run the task on this worker thread
            task();
        }
    }

    std::vector<std::jthread> threads;               // Modern C++20 automatically joining threads
    std::queue<std::function<void()>> tasks;        // Task queue
    std::mutex queue_mutex;                          // Protects the task queue
    std::condition_variable_any cv;                 // Modern C++20 CV that works with std::jthread stop_token
};

} // namespace lili
```

---

## 3. Why this is the "Modern Way"

1. **`std::jthread` instead of `std::thread`:**
   - Standard `std::thread` requires you to call `.join()` before they destruct, otherwise the program crashes.
   - `std::jthread` automatically signals a stop and joins in its destructor.
2. **`std::stop_token`:**
   - Provides a built-in, thread-safe way to request threads to exit without requiring custom boolean flags (`bool running`) or manually writing signal handling code.
3. **`std::condition_variable_any`:**
   - Standard `std::condition_variable` only works with `std::unique_lock<std::mutex>`.
   - `std::condition_variable_any` works with any lock and accepts a `std::stop_token`. The call `cv.wait(lock, stop_tok, predicate)` will interrupt waiting automatically when the thread pool is destroyed.

---

## 4. How to use it in Lili2D

### Example A: Asynchronous Chunk Mesh Rebuilding
Currently in [chunk.cpp](file:///home/lili/Documents/Lili2D/src/world/chunk.cpp#L78-L98), you do this:

```cpp
// ❌ Old Way: Spawns a brand-new OS thread every single time, which is slow!
rebuild_future = std::async(
    std::launch::async,
    [this, chunk_pos, tile_size, tiles_copy = tiles]() {
        return generateMeshData(chunk_pos, tile_size, tiles_copy);
    }
);
```

With the new `ThreadPool`, you can initialize a global or game-owned pool, and submit the task:

```cpp
// class Game holds std::unique_ptr<ThreadPool> thread_pool;

//  New Way: Reuses existing background worker threads
rebuild_future = game->getThreadPool()->enqueue(
    &Chunk::generateMeshData, this, chunk_pos, tile_size, tiles_copy
);
```

### Example B: Data-Parallel ECS System Updates
If you have a physics update loop in ECS that performs a lot of heavy mathematical operations (like kinematics, raycasting, or matrix math) on thousands of components:

```cpp
void PhysicsSystem::update(ECSRegistry& registry, float dt, ThreadPool& pool) {
    auto& components = registry.getPool<RigidBody>().getComponents();
    size_t total_components = components.size();
    
    // Split the work into 4 chunks (or based on hardware concurrency)
    size_t num_chunks = 4;
    size_t chunk_size = total_components / num_chunks;
    
    std::vector<std::future<void>> futures;
    
    for (size_t i = 0; i < num_chunks; ++i) {
        size_t start = i * chunk_size;
        size_t end = (i == num_chunks - 1) ? total_components : start + chunk_size;
        
        futures.push_back(pool.enqueue([&components, start, end, dt]() {
            for (size_t idx = start; idx < end; ++idx) {
                // Perform math on components[idx]
                components[idx].position += components[idx].velocity * dt;
            }
        }));
    }
    
    // Wait for all worker threads to finish processing their chunks
    for (auto& f : futures) {
        f.wait();
    }
}
```

> [!NOTE]
> When parallelizing loops, ensure that worker threads do not write to the same memory locations, and avoid modifying the registry topology (creating/destroying entities or adding/removing components) inside the worker threads, as this would cause data races.
