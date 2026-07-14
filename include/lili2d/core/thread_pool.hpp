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

/// @brief Settings for thread pool and rendering modes.
struct EngineConfig {
	PerformanceProfile profile = PerformanceProfile::CORRECT;

	size_t threads_to_leave_free = 1;
	size_t thread_count_override = 0;
	size_t max_thread_cap = 8;

	SDL_GPUPresentMode potato_present_mode = SDL_GPU_PRESENTMODE_VSYNC;
};

/// @brief A simple, lightweight C++20 Thread Pool using jthread and stop_token.
class ThreadPool {
public:
	explicit ThreadPool(const EngineConfig& config = {});

	ThreadPool(const ThreadPool&) = delete;
	ThreadPool& operator=(const ThreadPool&) = delete;
	ThreadPool(ThreadPool&&) = delete;
	ThreadPool& operator=(ThreadPool&&) = delete;

	/// @brief Enqueues a task for execution in the thread pool.
	/// @param task A void() function to execute.
	void enqueue(std::function<void()> task);

	/// @brief Returns the active performance profile.
	PerformanceProfile getProfile() const;

private:
	static size_t calculateThreadCount(const EngineConfig& config);
	void worker_loop(std::stop_token stop_tok);

	std::queue<std::function<void()>> tasks;
	std::mutex queue_mutex;
	std::condition_variable_any cv;
	std::vector<std::jthread> threads;
	PerformanceProfile profile;
};

} // namespace lili
