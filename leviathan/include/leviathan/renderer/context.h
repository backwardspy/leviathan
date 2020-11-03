#pragma once

#include "leviathan/lvpch.h"
#include "shader.h"
#include "vertex_array.h"
#include "vertex.h"

namespace lv {
    static std::vector<Index> auto_index(size_t count) {
        std::vector<Index> indices(count);
        std::iota(std::begin(indices), std::end(indices), 0);
        return indices;
    }

    class Context {
    public:
        virtual ~Context() noexcept {}

        virtual bool init() noexcept = 0;
        virtual void make_current() noexcept = 0;
        virtual void present() noexcept = 0;

        virtual std::shared_ptr<Shader> create_shader(Shader::SourceMap sources) noexcept = 0;
        virtual std::shared_ptr<VertexArray> create_vertex_array(std::vector<Vertex>&& vertices) noexcept = 0;
        virtual std::shared_ptr<VertexArray> create_vertex_array(std::vector<Vertex>&& vertices, std::vector<Index>&& indices) noexcept = 0;
    };
}
