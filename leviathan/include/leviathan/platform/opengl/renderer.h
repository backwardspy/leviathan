#pragma once

#include "leviathan/renderer.h"

namespace lv {
    namespace opengl {
        class RenderBackendImpl : public lv::RenderBackendImpl {
            virtual void set_clear_color(glm::vec4 color) noexcept override;
            virtual void clear() noexcept override;
            virtual void draw_vertex_array(VertexArray&, RenderMode) noexcept override;
        };
    }
}
