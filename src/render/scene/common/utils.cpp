#include "lili2d/render/scene/common/utils.hpp"

#include <cmath>

namespace lili {

MeshData
createUnitQuad() {
    MeshData quad;

    // Vertex (x, y, z, u, v, material_id)
    quad.vertices = {
        Vertex(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f),
        Vertex(1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f),
        Vertex(1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f),
        Vertex(0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f)
    };
    quad.indices = {0, 1, 2, 2, 3, 0};
    return quad;
}

MeshData
createUnitCircle(int segments) {
    MeshData circle;

    if (segments < 3) segments = 3;

    const float PI = 3.14159265359f;
    float angle_step = (2.0f * PI) / segments;

    circle.vertices.push_back(Vertex(0.0f, 0.0f, 0.0f, 0.5f, 0.5f, 0));

    for (int i = 0; i < segments; i++) {
        float angle = i * angle_step;

        float x = std::cos(angle) * 0.5f;
        float y = std::sin(angle) * 0.5f;

        float u = x + 0.5f;
        float v = y + 0.5f;

        circle.vertices.push_back(Vertex(x, y, 0.0f, u, v, 0));
    }

    for (int i = 1; i <= segments; i++) {
        uint32_t center_index = 0;
        uint32_t current_index = i;
        uint32_t next_index = (i == segments) ? 1 : (i + 1);

        circle.indices.push_back(center_index);
        circle.indices.push_back(current_index);
        circle.indices.push_back(next_index);
    }

    return circle;
}

}  // namespace lili
