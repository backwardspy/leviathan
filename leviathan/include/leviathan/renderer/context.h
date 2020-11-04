#pragma once

#include "leviathan/lvpch.h"
#include "shader.h"
#include "vertex_array.h"
#include "vertex.h"
#include "material.h"

namespace lv {
    static std::vector<Index> auto_index(size_t count) {
        std::vector<Index> indices(count);
        std::iota(std::begin(indices), std::end(indices), 0);
        return indices;
    }

    class Context {
    public:
        virtual ~Context() = default;

        virtual bool init() = 0;
        virtual void make_current() = 0;
        virtual void present() = 0;

        virtual std::shared_ptr<Shader> make_shader(Shader::SourceMap const&) = 0;
        virtual std::shared_ptr<VertexArray> make_vertex_array(std::vector<Vertex> const&&) = 0;
        virtual std::shared_ptr<VertexArray> make_vertex_array(std::vector<Vertex> const&&, std::vector<Index> const&&) = 0;
        std::shared_ptr<Material> make_material(std::shared_ptr<Shader>);
    };
}
