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

        void submit(Material& material, VertexArray& va, glm::mat4 model, glm::mat4 view_projection, RenderMode mode) {
            material.use();
            auto& shader = material.get_shader();
            shader.set_mat4("ViewProjection", view_projection);
            shader.set_mat4("Model", model);
            va.use();
            impl->draw_vertex_array(va, mode);
        }
    }
}