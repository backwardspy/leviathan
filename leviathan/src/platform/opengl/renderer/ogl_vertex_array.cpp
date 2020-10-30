#pragma once

#include "leviathan/lvpch.h"
#include "leviathan/exc.h"

#include "leviathan/platform/opengl/renderer/vertex_array.h"

namespace lv {
    namespace opengl {
        GLenum to_gl_type(ElementType type) {
            switch (type) {
                case ElementType::Float: return GL_FLOAT;
                case ElementType::UInt: return GL_UNSIGNED_INT;
                default: throw exc::EnumeratorNotImplementedError {};
            }
        }

        VertexArray::VertexArray(std::vector<Vertex>&& vertices, std::vector<Index>&& indices) : element_count { indices.size() } {
            glCreateVertexArrays(1, &vao);
            glBindVertexArray(vao);

            glCreateBuffers(1, &vbo);
            glBindBuffer(GL_ARRAY_BUFFER, vbo);
            glBufferData(GL_ARRAY_BUFFER, std::size(vertices) * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

            GLuint position = 0;
            size_t offset = 0;
            for (auto& element : element_layout<Vertex>()) {
                glEnableVertexAttribArray(position);
                glVertexAttribPointer(
                    position,
                    (GLint) element.components,
                    to_gl_type(element.type),
                    GL_FALSE,
                    sizeof(Vertex),
                    (const void*) offset
                );
                position++;
                offset += element.size;
            }

            glBindBuffer(GL_ARRAY_BUFFER, 0);

            glCreateBuffers(1, &ibo);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, std::size(indices) * sizeof(Index), indices.data(), GL_STATIC_DRAW);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

            glBindVertexArray(0);
        }

        VertexArray::~VertexArray() noexcept {
            glDeleteVertexArrays(1, &vao); vao = 0;
            glDeleteBuffers(1, &vbo); vbo = 0;
            glDeleteBuffers(1, &ibo); ibo = 0;
        }

        void VertexArray::use() noexcept {
            glBindVertexArray(vao);
            glBindBuffer(GL_ARRAY_BUFFER, vbo);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
        }
    }
}