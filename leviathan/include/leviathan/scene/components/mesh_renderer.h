#pragma once

#include "leviathan/renderer/material.h"
#include "leviathan/renderer/vertex_array.h"

namespace lv {
    struct MeshRenderer {
        ref<Material> material;
        ref<VertexArray> vertex_array;
    };
}
