#pragma once

#include "leviathan/renderer/renderer.h"

namespace lv {
    namespace opengl {
        class RenderBackendImpl : public lv::RenderBackendImpl {
            void set_clear_color(glm::vec4 color) override;
            void clear() override;
            void draw_vertex_array(VertexArray&, RenderMode) override;
        };
    }
}
