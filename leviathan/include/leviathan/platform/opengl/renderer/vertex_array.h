#pragma once

#include "leviathan/lvpch.h"
#include "leviathan/platform/opengl/opengl.h"
#include "leviathan/renderer/vertex_array.h"
#include "leviathan/renderer/vertex.h"

namespace lv {
    namespace opengl {
        class VertexArray : public lv::VertexArray {
        public:
            VertexArray(std::vector<Vertex>&& vertices, std::vector<Index>&& indices);

            void use() noexcept override;
            inline size_t get_element_count() const noexcept override { return element_count; }

            ~VertexArray() noexcept override;

        private:
            size_t element_count;
            GLuint vao, vbo, ibo;
        };
    }
}
