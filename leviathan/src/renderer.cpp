#include "leviathan/lvpch.h"
#include "leviathan/renderer/renderer.h"

#include "leviathan/platform/opengl/renderer/renderer.h"

namespace lv {
    std::unique_ptr<RenderBackendImpl> RenderBackend::impl = std::make_unique<opengl::RenderBackendImpl>();

    void Renderer::clear(glm::vec4 color) {
        RenderBackend::set_clear_color(color);
        RenderBackend::clear();
    }

    void Renderer::submit(Material& material, VertexArray& va, glm::mat4 transform, RenderMode mode) {
        material.use();
        material.get_shader().set_mat4("Model", transform);
        va.use();
        RenderBackend::draw_vertex_array(va, mode);
    }
}