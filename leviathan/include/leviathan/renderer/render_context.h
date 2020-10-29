#pragma once

#include "shader.h"
#include "buffer.h"
#include "vertex.h"

namespace lv {
    enum class RenderMode {
        Triangles,
    };

    class RenderContext {
    public:
        virtual ~RenderContext() noexcept {}

        virtual bool init() noexcept = 0;
        virtual void make_current() noexcept = 0;
        virtual void present() noexcept = 0;

#pragma region Resource Creation
        virtual std::unique_ptr<Shader> create_shader(Shader::SourceMap sources) noexcept = 0;
        virtual std::unique_ptr<VertexBuffer> create_vertex_buffer(std::vector<Vertex>&& vertices) noexcept = 0;
        virtual std::unique_ptr<IndexBuffer> create_index_buffer(std::vector<Index>&& indices) noexcept = 0;
#pragma endregion

#pragma region Rendering
        virtual void draw_indexed(RenderMode, VertexBuffer&, IndexBuffer&, Shader&) noexcept = 0;
#pragma endregion
    };
}
