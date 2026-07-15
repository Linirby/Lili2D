#include "lili2d/core/thread_pool.hpp"

#include <iostream>
#include <algorithm>
#include <utility>

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

void ThreadPool::enqueue(std::function<void()> task, TaskPriority priority) {
	{
		std::lock_guard<std::mutex> lock(queue_mutex);
		if (priority == TaskPriority::HIGH)
			high_tasks.push(std::move(task));
		else if (priority == TaskPriority::NORMAL)
			normal_tasks.push(std::move(task));
		else if (priority == TaskPriority::LOW)
			low_tasks.push(std::move(task));
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
			cv.wait(lock, stop_tok, [this] {
				return (
					!high_tasks.empty() ||
					!normal_tasks.empty() ||
					!low_tasks.empty()
				);
			});
			if (
				stop_tok.stop_requested() &&
				high_tasks.empty() && normal_tasks.empty() && low_tasks.empty()
			)
				return;
			if (!high_tasks.empty()) {
				task = std::move(high_tasks.front());
				high_tasks.pop();
			} else if (!normal_tasks.empty()) {
				task = std::move(normal_tasks.front());
				normal_tasks.pop();
			} else if (!low_tasks.empty()) {
				task = std::move(low_tasks.front());
				low_tasks.pop();
			}
		}
		try {
			task();
		} catch(const std::exception &e) {
			std::cerr << "[ThreadPool Worker Error]: " << e.what() << "\n";
		}
	}
}

}  // namespace lili
