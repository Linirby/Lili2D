#pragma once

#include <SDL3/SDL.h>

#include <condition_variable>
#include <functional>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

namespace lili {

/// @brief Performance profile options to scale threading and rendering
/// strategies.
enum class PerformanceProfile {
    YES,      ///< Standard performance profile.
    CORRECT,  ///< Balanced performance and resource efficiency profile.
    INSANE    ///< Maximum throughput high-performance profile.
};

/// @brief Task priority options for tasks in a thread pool
enum class TaskPriority {
    HIGH,    ///< High priority task executed first.
    NORMAL,  ///< Normal priority task.
    LOW      ///< Low priority background task.
};

/// @brief Settings for thread pool and rendering modes.
struct EngineConfig {
    /// @brief Target performance profile.
    PerformanceProfile profile = PerformanceProfile::CORRECT;

    /// @brief Number of CPU core threads to leave unallocated for system
    /// processes.
    size_t threads_to_leave_free = 1;
    /// @brief Explicit override for total worker thread count (0 uses auto
    /// calculation).
    size_t thread_count_override = 0;
    /// @brief Maximum cap on worker thread count.
    size_t max_thread_cap = 8;

    /// @brief GPU present mode configuration.
    SDL_GPUPresentMode potato_present_mode = SDL_GPU_PRESENTMODE_VSYNC;
};

/// @brief A simple, lightweight C++20 Thread Pool using jthread and
/// stop_token.
class ThreadPool {
public:
    /// @brief Constructs a ThreadPool with specified engine configuration.
    /// @param config Engine configuration settings.
    explicit ThreadPool(const EngineConfig& config = {});

    /// @brief Deleted copy constructor.
    ThreadPool(const ThreadPool&) = delete;
    /// @brief Deleted copy assignment operator.
    ThreadPool&
    operator=(const ThreadPool&) = delete;
    /// @brief Deleted move constructor.
    ThreadPool(ThreadPool&&) = delete;
    /// @brief Deleted move assignment operator.
    ThreadPool&
    operator=(ThreadPool&&) = delete;

    /// @brief Enqueues a task for execution in the thread pool.
    /// @param task A void() function to execute.
    /// @param priority Priority level of the task (default:
    /// TaskPriority::NORMAL).
    void
    enqueue(
        std::function<void()> task, TaskPriority priority = TaskPriority::NORMAL
    );

    /// @brief Returns the active performance profile.
    /// @return Active PerformanceProfile.
    [[nodiscard]] inline PerformanceProfile
    getProfile() const noexcept {
        return profile;
    }

private:
    /// @brief Calculates optimal worker thread count based on engine config.
    /// @param config Engine configuration.
    /// @return Calculated thread count.
    static size_t
    calculateThreadCount(const EngineConfig& config);
    /// @brief Worker thread loop function.
    /// @param stop_tok Stop token to observe cancellation requests.
    void
    worker_loop(std::stop_token stop_tok);

    /// @brief Queue for high-priority tasks.
    std::queue<std::function<void()>> high_tasks;
    /// @brief Queue for normal-priority tasks.
    std::queue<std::function<void()>> normal_tasks;
    /// @brief Queue for low-priority tasks.
    std::queue<std::function<void()>> low_tasks;
    /// @brief Mutex synchronizing task queue access.
    std::mutex queue_mutex;
    /// @brief Condition variable for worker thread notifications.
    std::condition_variable_any cv;
    /// @brief Worker thread pool array.
    std::vector<std::jthread> threads;
    /// @brief Performance profile setting.
    PerformanceProfile profile;
};

}  // namespace lili
