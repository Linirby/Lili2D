#include "lili2d/ecs/command_buffer.hpp"
#include "lili2d/ecs/ecs_registry.hpp"

namespace lili {

void CommandBuffer::createEntity() {
    commands.push_back([](ECSRegistry &registry) {
        registry.createEntity();
    });
}

void CommandBuffer::destroyEntity(Entity entity) {
	commands.push_back([entity](ECSRegistry& registry) {
		registry.destroyEntity(entity);
	});
}

void CommandBuffer::play(ECSRegistry& registry) {
	for (auto& command : commands) {
		command(registry);
	}
	commands.clear();
}

void CommandBuffer::clear() {
	commands.clear();
}

}  // namespace lili
