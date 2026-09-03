#pragma once

#include <cstddef>
#include <iterator>
#include <tuple>

#include "lili2d/ecs/component_pool.hpp"
#include "lili2d/ecs/ecs_registry.hpp"
#include "lili2d/ecs/entity.hpp"

namespace lili {

template <typename... Components>
class ECSView {
public:
    class Iterator {
    public:
        using iterator_category = std::forward_iterator_tag;
        using difference_type = std::ptrdiff_t;
        using value_type = std::tuple<Entity, Components&...>;
        using pointer = void;
        using reference = value_type;

        Iterator(
            std::tuple<ComponentPool<Components>*...> pool_ptrs,
            const IComponentPool* lead_pool, size_t index, size_t max_size
        )
            : pool_ptrs(pool_ptrs),
              lead_pool(lead_pool),
              lead_entities(
                  lead_pool ? lead_pool->getEntities().data() : nullptr
              ),
              index(index),
              max_size(max_size) {
            find_valid();
        }

        [[nodiscard]] reference
        operator*() const {
            return std::apply(
                [this](ComponentPool<Components>*... pools) {
                    auto get_comp = [this](auto* pool) -> decltype(auto) {
                        if (pool == lead_pool) {
                            return pool->getComponents()[index];
                        }
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
        operator++() {
            ++index;
            find_valid();
            return *this;
        }

        Iterator
        operator++(int) {
            Iterator tmp = *this;
            ++(*this);
            return tmp;
        }

        [[nodiscard]] bool
        operator==(const Iterator& other) const noexcept {
            return index == other.index;
        }

        [[nodiscard]] bool
        operator!=(const Iterator& other) const noexcept {
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
        find_valid() {
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
                if (valid) return;
                ++index;
            }
        }
    };

    explicit ECSView(ECSRegistry& registry)
        : pool_ptrs(&registry.getPool<Components>()...) {
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
    begin() const {
        if (!lead_pool || lead_pool->empty()) return end();
        return Iterator(pool_ptrs, lead_pool, 0, lead_pool->size());
    }

    [[nodiscard]] Iterator
    end() const {
        size_t end_idx = lead_pool ? lead_pool->size() : 0;
        return Iterator(pool_ptrs, lead_pool, end_idx, end_idx);
    }

    [[nodiscard]] bool
    empty() const noexcept {
        return begin() == end();
    }

private:
    std::tuple<ComponentPool<Components>*...> pool_ptrs;
    const IComponentPool* lead_pool = nullptr;
};

}  // namespace lili
