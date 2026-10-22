#pragma once

#include "components.hpp"
#include <lili2d/components/render2d.hpp>
#include <lili2d/components/transform.hpp>
#include <lili2d/core/thread_pool.hpp>
#include <lili2d/ecs.hpp>
#include <lili2d/ecs/ecs_registry.hpp>
#include <lili2d/geometry/vec2.hpp>

class MovementSystem
  : public lili::System<
        lili::TransformComponent,
        VelocityComponent,
        lili::Render2DComponent>
{
public:
    static void
    update(
        lili::ECSRegistry& registry,
        lili::Vec2 window_size,
        float dt,
        lili::ThreadPool* thread_pool
    )
    {
        MovementSystem sys(window_size);
        sys.run(registry, dt, thread_pool);
    }

    MovementSystem(lili::Vec2 window_size);

    void
    updateEntity(
        lili::ECSRegistry& registry,
        lili::Entity entity,
        lili::TransformComponent& trans,
        VelocityComponent& vel,
        lili::Render2DComponent& render2d,
        float dt
    ) override;

private:
    float window_w;
    float window_h;
};
