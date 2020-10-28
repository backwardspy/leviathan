#include "leviathan/lvpch.h"

#include <glm/glm.hpp>

#include "leviathan/log.h"
#include "leviathan/layers/debug_gl_layer.h"

namespace lv {
    void DebugGLLayer::init() noexcept {
        glCreateVertexArrays(1, &vao);
        glBindVertexArray(vao);

        glCreateBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);

        glCreateBuffers(1, &ibo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);

        glm::vec3 verts[3] = {
            { 0.0f, 0.5f, 0.0f },
            { 0.5f, -0.4f, 0.0f },
            { -0.5f, -0.4f, 0.0f },
        };

        GLuint indices[3] = { 0, 1, 2 };

        glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * 3, verts, GL_STATIC_DRAW);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * 3, indices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), 0);
        glEnableVertexAttribArray(0);
    }

    void DebugGLLayer::render() noexcept {
        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
    }
}