#pragma once

#include "leviathan/lvpch.h"
#include "vertex_array.h"
#include "context.h"
#include "material.h"

namespace lv {
    enum class RenderMode {
        Triangles,
    };

    namespace renderer {
        void clear(glm::vec4 color = glm::vec4(0, 0, 0, 1));
        void submit(Material& material, VertexArray& va, glm::mat4 model, glm::mat4 view_projection, RenderMode mode = RenderMode::Triangles);
    }
}
