#pragma once

#include "leviathan/renderer/material.h"
#include "leviathan/renderer/vertex_array.h"

namespace lv {
    namespace ecs {
        struct Mesh {
            std::shared_ptr<Material> material;
            std::shared_ptr<VertexArray> vertex_array;
        };
    }
}
