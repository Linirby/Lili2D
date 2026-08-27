#include "lili2d/render/scene/common/animation_registry.hpp"

namespace lili {

AnimationRegistry&
AnimationRegistry::get() {
    static AnimationRegistry instance;
    return instance;
}

AnimationRegistry::AnimationRegistry() {
    registerAnimation("core:none", Animation());
}

}  // namespace lili
