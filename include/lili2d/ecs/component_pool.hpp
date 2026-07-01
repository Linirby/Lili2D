#pragma once

#include <vector>
#include "lili2d/ecs/entity.hpp"

namespace lili {

class IComponentPool {
public:
	virtual ~IComponentPool() = default;

	virtual bool has(Entity entity) const = 0;
	virtual void remove(Entity entity) = 0;
};

template<typename T>
class ComponentPool : IComponentPool {
public:
	template<typename ...Args>
	T &emplace(Entity entity, Args &&...args);
	T &get(Entity entity);
	bool has(Entity entity) const override;
	void remove(Entity entity) override;

private:
	std::vector<T> dense_components;
	std::vector<Entity> dense_entities;
	std::vector<size_t> sparse_entities;
	static constexpr size_t TOMBSTONE = static_cast<size_t>(-1);
};

}  // namespace lili
