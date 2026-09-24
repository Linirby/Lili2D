#include "lili2d/render/scene/common/material.hpp"

namespace lili {

void
Material::setVertexUniformsRaw(const void* data, size_t size)
{
    const auto* ptr = static_cast<const uint8_t*>(data);
    custom_vertex_uniforms.assign(ptr, ptr + size);
}

void
Material::setFragmentUniformsRaw(const void* data, size_t size)
{
    const auto* ptr = static_cast<const uint8_t*>(data);
    custom_fragment_uniforms.assign(ptr, ptr + size);
}

} // namespace lili
