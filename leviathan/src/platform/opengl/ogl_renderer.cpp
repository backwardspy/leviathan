#include "leviathan/lvpch.h"
#include "leviathan/platform/opengl/opengl.h"
#include "leviathan/platform/opengl/renderer.h"
#include "leviathan/exc.h"

namespace lv {
    namespace opengl {
        constexpr GLenum to_gl_mode(RenderMode mode) {
            switch (mode) {
                case RenderMode::Triangles: return GL_TRIANGLES;
                default: throw exc::EnumeratorNotImplementedError {};
            }
        }

        void RenderBackendImpl::set_clear_color(glm::vec4 color) noexcept {
            glClearColor(color.r, color.g, color.b, color.a);
        }

        void RenderBackendImpl::clear() noexcept {
            glClear(GL_COLOR_BUFFER_BIT);
        }

        void RenderBackendImpl::draw_vertex_array(VertexArray& va, RenderMode mode) {
            glDrawElements(to_gl_mode(mode), (GLsizei) va.get_element_count(), GL_UNSIGNED_INT, nullptr);
        }
    }
}