#pragma once

#include "leviathan/renderer/renderer.h"

namespace lv {
    namespace opengl {
        class RenderBackendImpl {
        public:
            void set_clear_color(glm::vec4 color);
            void clear();
            void draw_vertex_array(VertexArray&, RenderMode);
        };
    }
}
