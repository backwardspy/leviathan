#pragma once

#include <glm/glm.hpp>

namespace lv {
    using VertexPosition = glm::vec3;

    struct Vertex {
        VertexPosition position;
    };

    using Index = uint32_t;
}
