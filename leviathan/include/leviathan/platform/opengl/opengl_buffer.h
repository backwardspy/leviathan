#pragma once

#include "leviathan/lvpch.h"

#include "leviathan/renderer/buffer.h"
#include "leviathan/renderer/vertex.h"

namespace lv {
    namespace opengl {
        class VertexBuffer : public lv::VertexBuffer {
        public:
            VertexBuffer(std::vector<Vertex>&& vertices) noexcept;

            virtual void use() noexcept override;

            ~VertexBuffer() noexcept;

        private:
            GLuint vbo, vao;
        };

        class IndexBuffer : public lv::IndexBuffer {
        public:
            IndexBuffer(std::vector<Index>&& indices) noexcept;

            virtual size_t get_element_count() const noexcept override { return count; }

            virtual void use() noexcept override;

            ~IndexBuffer() noexcept;

        private:
            GLuint handle;
            size_t count;
        };
    }
}
