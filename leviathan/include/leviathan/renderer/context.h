#pragma once

#include "leviathan/lvpch.h"
#include "shader.h"
#include "vertex_array.h"
#include "vertex.h"
#include "material.h"
#include "texture.h"

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

        virtual ref<Shader> make_shader(std::string const&) = 0;
        virtual ref<VertexArray> make_vertex_array(std::vector<Vertex>&&) = 0;
        virtual ref<VertexArray> make_vertex_array(std::vector<Vertex>&&, std::vector<Index>&&) = 0;
        virtual ref<Texture> make_texture(std::string const& filename) = 0;
        ref<Material> make_material(ref<Shader>);
    };
}
