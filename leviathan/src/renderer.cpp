#include "leviathan/lvpch.h"
#include "leviathan/renderer/renderer.h"

#include "leviathan/platform/opengl/renderer.h"

namespace lv {
    std::unique_ptr<RenderBackendImpl> RenderBackend::impl = std::make_unique<opengl::RenderBackendImpl>();

    void Renderer::clear(glm::vec4 color) noexcept {
        RenderBackend::set_clear_color(color);
        RenderBackend::clear();
    }

    void Renderer::submit(Shader& shader, VertexArray& va, glm::mat4 transform, RenderMode mode) noexcept {
        shader.use();
        shader.set_mat4("Model", transform);
        va.use();
        RenderBackend::draw_vertex_array(va, mode);
    }
}