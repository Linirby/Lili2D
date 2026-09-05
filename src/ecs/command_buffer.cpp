#include "lili2d/ecs/command_buffer.hpp"

namespace lili {

void
CommandBuffer::createEntity() {
    commands.push_back([](ECSRegistry& registry) {
        (void)registry.createEntity();
    });
}

void
CommandBuffer::destroyEntity(Entity entity) {
    commands.push_back([entity](ECSRegistry& registry) {
        registry.destroyEntity(entity);
    });
}

void
CommandBuffer::play(ECSRegistry& registry) {
    for (auto& command : commands) command(registry);
    commands.clear();
}

void
CommandBuffer::clear() noexcept {
    commands.clear();
}

}  // namespace lili
