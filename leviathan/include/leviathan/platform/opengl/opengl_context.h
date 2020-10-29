#pragma once

#include <GLFW/glfw3.h>

#include "leviathan/renderer/render_context.h"

namespace lv {
    class OpenGLContext : public RenderContext {
    public:
        OpenGLContext(GLFWwindow* handle) noexcept;

        virtual bool init() noexcept override;
        virtual void make_current() noexcept override;
        virtual void present() noexcept override;

#pragma region Resource Creation
        virtual std::unique_ptr<Shader> create_shader(Shader::SourceMap sources) noexcept override;
        virtual std::unique_ptr<VertexBuffer> create_vertex_buffer(std::vector<Vertex>&& vertices) noexcept override;
        virtual std::unique_ptr<IndexBuffer> create_index_buffer(std::vector<Index>&& indices) noexcept override;
#pragma endregion

#pragma region Rendering
        virtual void draw_indexed(RenderMode, VertexBuffer&, IndexBuffer&, Shader&) noexcept override;
#pragma endregion

    private:
        GLFWwindow* handle;
    };
}
