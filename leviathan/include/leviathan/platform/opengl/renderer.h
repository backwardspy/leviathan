#pragma once

#include "leviathan/renderer/renderer.h"

namespace lv {
    namespace opengl {
        class RenderBackendImpl : public lv::RenderBackendImpl {
            void set_clear_color(glm::vec4 color) noexcept override;
            void clear() noexcept override;
            void draw_vertex_array(VertexArray&, RenderMode) override;
        };
    }
}
