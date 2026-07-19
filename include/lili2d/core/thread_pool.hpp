#pragma once

#include <SDL3/SDL.h>
#include <vector>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <functional>

namespace lili {

/// @brief Performance profile options to scale threading and rendering
/// strategies.
enum class PerformanceProfile {
	YES,
	CORRECT,
	INSANE
};

/// @brief Task priority options for tasks in a thread pool
enum class TaskPriority {
	HIGH,
	NORMAL,
	LOW
};

/// @brief Settings for thread pool and rendering modes.
struct EngineConfig {
	/// @brief The active performance profile.
	PerformanceProfile profile = PerformanceProfile::CORRECT;

	/// @brief Number of CPU cores to leave free for OS/other tasks.
	size_t threads_to_leave_free = 1;
	/// @brief Explicit number of threads to spawn. If 0, uses automatic detection.
	size_t thread_count_override = 0;
	/// @brief Hard limit on the maximum number of worker threads to spawn.
	size_t max_thread_cap = 8;

	/// @brief The fallback GPU present mode if the system has poor performance.
	SDL_GPUPresentMode potato_present_mode = SDL_GPU_PRESENTMODE_VSYNC;
};

/// @brief A simple, lightweight C++20 Thread Pool using jthread and stop_token.
class ThreadPool {
public:
	/// @brief Constructs the ThreadPool with a given config.
	/// @param config The engine configuration settings.
	explicit ThreadPool(const EngineConfig& config = {});

	ThreadPool(const ThreadPool&) = delete;
	ThreadPool& operator=(const ThreadPool&) = delete;
	ThreadPool(ThreadPool&&) = delete;
	ThreadPool& operator=(ThreadPool&&) = delete;

	/// @brief Enqueues a task for execution in the thread pool.
	/// @param task A void() function to execute.
	/// @param priority The priority of the task.
	void enqueue(
		std::function<void()> task, TaskPriority priority = TaskPriority::NORMAL
	);

	/// @brief Returns the active performance profile.
	PerformanceProfile getProfile() const;

private:
	static size_t calculateThreadCount(const EngineConfig& config);
	void worker_loop(std::stop_token stop_tok);

	std::queue<std::function<void()>> high_tasks;
	std::queue<std::function<void()>> normal_tasks;
	std::queue<std::function<void()>> low_tasks;
	std::mutex queue_mutex;
	std::condition_variable_any cv;
	std::vector<std::jthread> threads;
	PerformanceProfile profile;
};

} // namespace lili
