#pragma once

#include "leviathan/lvpch.h"

#include "leviathan/platform/opengl/opengl_buffer.h"

namespace lv {
    namespace opengl {
        VertexBuffer::VertexBuffer(std::vector<Vertex>&& vertices) noexcept {
            glCreateBuffers(1, &vbo);
            glCreateVertexArrays(1, &vao);
            use();
            glBufferData(GL_ARRAY_BUFFER, std::size(vertices) * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

            // position
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(
                0,
                (GLint) VertexPosition::length(),
                GL_FLOAT,
                GL_FALSE,
                sizeof(Vertex),
                (const void*) offsetof(Vertex, position)
            );
        }

        VertexBuffer::~VertexBuffer() noexcept {
            glDeleteBuffers(1, &vbo);
        }

        void VertexBuffer::use() noexcept {
            glBindBuffer(GL_ARRAY_BUFFER, vbo);
            glBindVertexArray(vao);
        }

        IndexBuffer::IndexBuffer(std::vector<Index>&& indices) noexcept :
            count { indices.size() } {
            glCreateBuffers(1, &handle);
            use();
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(Index), indices.data(), GL_STATIC_DRAW);
        }

        IndexBuffer::~IndexBuffer() noexcept {
            glDeleteBuffers(1, &handle);
        }

        void IndexBuffer::use() noexcept {
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, handle);
        }
    }
}