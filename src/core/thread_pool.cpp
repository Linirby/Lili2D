#include "lili2d/core/thread_pool.hpp"

#include <iostream>
#include <algorithm>

namespace lili {

ThreadPool::ThreadPool(const EngineConfig& config) : profile(config.profile) {
	size_t num_threads = calculateThreadCount(config);
	threads.reserve(num_threads);
	for (size_t i = 0; i < num_threads; ++i) {
		threads.emplace_back([this](std::stop_token stop_tok) {
			worker_loop(stop_tok);
		});
	}
}

void ThreadPool::enqueue(std::function<void()> task) {
	{
		std::lock_guard<std::mutex> lock(queue_mutex);
		tasks.push(std::move(task));
	}
	cv.notify_one();
}

PerformanceProfile ThreadPool::getProfile() const {
	return profile;
}

size_t ThreadPool::calculateThreadCount(const EngineConfig& config) {
	if (config.thread_count_override > 0)
		return config.thread_count_override;

	size_t hw = std::thread::hardware_concurrency();
	if (hw == 0)
		hw = 1;

	if (config.profile == PerformanceProfile::YES)
		return 1;

	size_t count = hw;
	if (count > config.threads_to_leave_free)
		count -= config.threads_to_leave_free;
	else
		count = 1;

	if (config.profile == PerformanceProfile::CORRECT)
		count = std::min(count, config.max_thread_cap);
	else if (config.profile == PerformanceProfile::INSANE)
		count = std::min(count, static_cast<size_t>(16));

	if (count == 0)
		count = 1;
	return count;
}

void ThreadPool::worker_loop(std::stop_token stop_tok) {
	while (true) {
		std::function<void()> task;
		{
			std::unique_lock<std::mutex> lock(queue_mutex);
			cv.wait(lock, stop_tok, [this] { return !tasks.empty(); });
			if (stop_tok.stop_requested() && tasks.empty())
				return;
			task = std::move(tasks.front());
			tasks.pop();
		}
		try {
			task();
		} catch(const std::exception &e) {
			std::cerr << "[ThreadPool Worker Error]: " << e.what() << "\n";
		}
	}
}

}  // namespace lili
