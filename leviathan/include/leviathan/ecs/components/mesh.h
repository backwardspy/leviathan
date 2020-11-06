#pragma once

#include "leviathan/core.h"
#include "leviathan/renderer/material.h"
#include "leviathan/renderer/vertex_array.h"

namespace lv {
    namespace ecs {
        struct Mesh {
            ref<Material> material;
            ref<VertexArray> vertex_array;
        };
    }
}
