#pragma once

#include "components.hpp"
#include <lili2d/components/render2d.hpp>
#include <lili2d/components/transform.hpp>
#include <lili2d/ecs/system.hpp>

class MovementSystem
  : public lili::System<lili::cTransform, VelocityComponent, lili::cRender2D>
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
        lili::cTransform& trans,
        VelocityComponent& vel,
        lili::cRender2D& render2d,
        float dt
    ) override;

private:
    float window_w;
    float window_h;
};
