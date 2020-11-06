#pragma once

#include "leviathan/lvpch.h"
#include "vertex_array.h"
#include "context.h"
#include "material.h"

namespace lv {
    enum class RenderMode {
        Triangles,
    };

    class Renderer {
    public:
        static void clear(glm::vec4 color = glm::vec4(0, 0, 0, 1));
        static void submit(Material&, VertexArray&, glm::mat4 transform, RenderMode = RenderMode::Triangles);
    };

    class RenderBackendImpl {
    public:
        virtual void set_clear_color(glm::vec4 color) = 0;
        virtual void clear() = 0;
        virtual void draw_vertex_array(VertexArray&, RenderMode) = 0;
    };

    class RenderBackend {
    public:
        static void set_clear_color(glm::vec4 color) { impl->set_clear_color(color); }
        static void clear() { impl->clear(); }
        static void draw_vertex_array(VertexArray& va, RenderMode mode) { impl->draw_vertex_array(va, mode); }

    private:
        static scope<RenderBackendImpl> impl;
    };
}
