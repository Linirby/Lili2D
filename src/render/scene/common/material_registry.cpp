#include "lili2d/render/scene/common/material_registry.hpp"

namespace lili {

MaterialRegistry&
MaterialRegistry::get() {
    static MaterialRegistry instance;
    return instance;
}

MaterialRegistry::MaterialRegistry() {
    registerMaterial("core:none", Material());
}

}  // namespace lili
