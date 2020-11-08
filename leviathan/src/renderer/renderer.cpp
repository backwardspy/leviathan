#include "leviathan/lvpch.h"
#include "leviathan/renderer/renderer.h"

#if defined(LV_GRAPHICS_OPENGL)
#include "leviathan/platform/opengl/renderer/renderer.h"
using RenderBackendImpl = lv::opengl::RenderBackendImpl;
#endif

namespace lv {
    namespace renderer {
        static scope<RenderBackendImpl> impl = make_scope<RenderBackendImpl>();

        void clear(glm::vec4 color) {
            impl->set_clear_color(color);
            impl->clear();
        }

        void submit(Material& material, VertexArray& va, glm::mat4 transform, RenderMode mode) {
            material.use();
            material.get_shader().set_mat4("Model", transform);
            va.use();
            impl->draw_vertex_array(va, mode);
        }
    }
}