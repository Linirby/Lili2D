#pragma once

#include <algorithm>
#include <cstddef>
#include <future>
#include <iterator>
#include <memory>
#include <tuple>
#include <vector>

#include "lili2d/core/thread_pool.hpp"
#include "lili2d/ecs/component_pool.hpp"
#include "lili2d/ecs/ecs_registry.hpp"
#include "lili2d/ecs/entity.hpp"

namespace lili {

template<typename... Components>
class ECSView
{
public:
    class Iterator
    {
    public:
        using iterator_category = std::forward_iterator_tag;
        using difference_type = std::ptrdiff_t;
        using value_type = std::tuple<Entity, Components&...>;
        using pointer = void;
        using reference = value_type;

        Iterator(
            std::tuple<ComponentPool<Components>*...> pool_ptrs,
            const IComponentPool* lead_pool,
            size_t index,
            size_t max_size
        )
          : pool_ptrs(pool_ptrs)
          , lead_pool(lead_pool)
          , lead_entities(lead_pool ? lead_pool->getEntities().data() : nullptr)
          , index(index)
          , max_size(max_size)
        {
            find_valid();
        }

        [[nodiscard]] reference
        operator*() const
        {
            return std::apply(
                [this](ComponentPool<Components>*... pools) {
                    auto get_comp = [this](auto* pool) -> decltype(auto) {
                        if (pool == lead_pool)
                            return pool->getComponents()[index];
                        return pool->get(current_entity);
                    };
                    return std::tuple<Entity, Components&...>(
                        current_entity, get_comp(pools)...
                    );
                },
                pool_ptrs
            );
        }

        Iterator&
        operator++()
        {
            ++index;
            find_valid();
            return *this;
        }

        Iterator
        operator++(int)
        {
            Iterator tmp = *this;
            ++(*this);
            return tmp;
        }

        [[nodiscard]] bool
        operator==(const Iterator& other) const noexcept
        {
            return index == other.index;
        }

        [[nodiscard]] bool
        operator!=(const Iterator& other) const noexcept
        {
            return !(*this == other);
        }

    private:
        std::tuple<ComponentPool<Components>*...> pool_ptrs;
        const IComponentPool* lead_pool = nullptr;
        const Entity* lead_entities = nullptr;
        size_t index = 0;
        size_t max_size = 0;
        Entity current_entity = 0;

        void
        find_valid()
        {
            while (index < max_size) {
                current_entity = lead_entities[index];
                bool valid = std::apply(
                    [this](ComponentPool<Components>*... pool) {
                        return (
                            ((pool == lead_pool) ||
                             pool->has(current_entity)) &&
                            ...
                        );
                    },
                    pool_ptrs
                );
                if (valid)
                    return;
                ++index;
            }
        }
    };

    explicit ECSView(ECSRegistry& registry)
      : pool_ptrs(&registry.getPool<Components>()...)
    {
        std::apply(
            [this](ComponentPool<Components>*... pools) {
                auto inspect = [this](const IComponentPool* pool) {
                    if (!lead_pool || pool->size() < lead_pool->size())
                        lead_pool = pool;
                };
                (inspect(pools), ...);
            },
            pool_ptrs
        );
    }

    [[nodiscard]] Iterator
    begin() const
    {
        if (!lead_pool || lead_pool->empty())
            return end();
        return Iterator(pool_ptrs, lead_pool, 0, lead_pool->size());
    }

    [[nodiscard]] Iterator
    end() const
    {
        size_t end_idx = lead_pool ? lead_pool->size() : 0;
        return Iterator(pool_ptrs, lead_pool, end_idx, end_idx);
    }

    [[nodiscard]] bool
    empty() const noexcept
    {
        return begin() == end();
    }

    /// @brief Executes a callback sequentially for each matching entity.
    /// @tparam Func Callable accepting (Entity, Components&...).
    /// @param fn The callback to invoke.
    template<typename Func>
    void
    each(Func&& fn) const
    {
        if (!lead_pool || lead_pool->empty())
            return;
        const auto& entities = lead_pool->getEntities();
        runChunk(entities, 0, entities.size(), fn);
    }

    /// @brief Executes a callback across worker threads when entity count
    /// exceeds parallel_threshold, or sequentially otherwise.
    /// @tparam Func Callable accepting (Entity, Components&...).
    /// @param thread_pool Pointer to ThreadPool (if nullptr, runs sequentially).
    /// @param fn The callback to invoke per entity.
    /// @param parallel_threshold Minimum lead pool size to trigger parallel run.
    template<typename Func>
    void
    parallelEach(
        ThreadPool* thread_pool,
        Func&& fn,
        size_t parallel_threshold = 1000
    ) const
    {
        if (!lead_pool || lead_pool->empty())
            return;

        const auto& entities = lead_pool->getEntities();
        size_t total = entities.size();

        if (!thread_pool ||
            thread_pool->getProfile() == PerformanceProfile::YES ||
            total < parallel_threshold) {
            runChunk(entities, 0, total, fn);
            return;
        }

        size_t num_workers = thread_pool->getThreadCount();
        size_t chunk_size = (total + num_workers - 1) / num_workers;
        std::vector<std::future<void>> futures;
        futures.reserve(num_workers);

        for (size_t start = 0; start < total; start += chunk_size) {
            size_t end = std::min(start + chunk_size, total);
            auto promise = std::make_shared<std::promise<void>>();
            futures.push_back(promise->get_future());

            thread_pool->enqueue(
                [this, &entities, start, end, &fn, promise]() {
                    runChunk(entities, start, end, fn);
                    promise->set_value();
                },
                TaskPriority::HIGH
            );
        }

        for (auto& f : futures)
            f.wait();
    }

private:
    std::tuple<ComponentPool<Components>*...> pool_ptrs;
    const IComponentPool* lead_pool = nullptr;

    template<typename Func>
    void
    runChunk(
        const std::vector<Entity>& entities,
        size_t start,
        size_t end,
        Func& fn
    ) const
    {
        for (size_t i = start; i < end; ++i) {
            Entity entity = entities[i];
            std::apply(
                [&](ComponentPool<Components>*... pools) {
                    bool has_all =
                        (((pools == lead_pool) || pools->has(entity)) && ...);
                    if (has_all) {
                        auto get_comp = [&](auto* pool) -> decltype(auto) {
                            if (pool == lead_pool)
                                return pool->getComponents()[i];
                            return pool->get(entity);
                        };
                        fn(entity, get_comp(pools)...);
                    }
                },
                pool_ptrs
            );
        }
    }
};

} // namespace lili
