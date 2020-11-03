#pragma once

#include "leviathan/renderer/shader.h"
#include "leviathan/renderer/vertex_array.h"

namespace lv {
    namespace ecs {
        struct Mesh {
            std::shared_ptr<Shader> shader;
            std::shared_ptr<VertexArray> vertex_array;
        };
    }
}
