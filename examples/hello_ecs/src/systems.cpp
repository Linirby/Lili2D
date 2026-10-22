#include "systems.hpp"

#include "components.hpp"
#include <lili2d/components/circle.hpp>
#include <lili2d/components/render2d.hpp>
#include <lili2d/components/sprite.hpp>
#include <lili2d/components/transform.hpp>
#include <lili2d/ecs/ecs_view.hpp>
#include <lili2d/ecs/entity.hpp>
#include <lili2d/geometry/vec2.hpp>
#include <lili2d/render/gpu/pass_types.hpp>

using namespace lili;

namespace systems {

void
updateMovement(ECSRegistry& registry, float dt, float window_w, float window_h)
{
    auto view =
        registry
            .view<TransformComponent, VelocityComponent, Render2DComponent>();
    for (auto&& [entity, trans, vel, render2d] : view) {
        trans.pos += vel.value * dt;

        Vec2 final_size = Vec2(
            render2d.size.x * trans.scale.x * 0.5f,
            render2d.size.y * trans.scale.y * 0.5f
        );
        if (trans.pos.x - final_size.x < 0.0f) {
            trans.pos.x = final_size.x;
            vel.value.x = -vel.value.x;
        } else if (trans.pos.x + final_size.x > window_w) {
            trans.pos.x = window_w - final_size.x;
            vel.value.x = -vel.value.x;
        }

        if (trans.pos.y - final_size.y < 0.0f) {
            trans.pos.y = final_size.y;
            vel.value.y = -vel.value.y;
        } else if (trans.pos.y + final_size.y > window_h) {
            trans.pos.y = window_h - final_size.y;
            vel.value.y = -vel.value.y;
        }
    }
}

} // namespace systems
