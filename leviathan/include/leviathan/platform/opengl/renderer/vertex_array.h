#pragma once

#include "leviathan/lvpch.h"
#include "leviathan/platform/opengl/opengl.h"
#include "leviathan/renderer/vertex_array.h"
#include "leviathan/renderer/vertex.h"

namespace lv {
    namespace opengl {
        class VertexArray : public lv::VertexArray {
        public:
            VertexArray(std::vector<Vertex> const&& vertices, std::vector<Index> const&& indices);

            void use() override;
            size_t get_element_count() const override { return element_count; }

            ~VertexArray() override;

        private:
            size_t element_count;
            GLuint vao, vbo, ibo;
        };
    }
}
