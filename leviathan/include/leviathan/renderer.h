#pragma once

#include "leviathan/lvpch.h"
#include "renderer/shader.h"
#include "renderer/vertex_array.h"

namespace lv {
    enum class RenderMode {
        Triangles,
    };

    class Renderer {
    public:
        static void clear(glm::vec4 color = glm::vec4(0, 0, 0, 1)) noexcept;
        static void submit(Shader&, VertexArray&, RenderMode = RenderMode::Triangles) noexcept;
    };

    class RenderBackendImpl {
    public:
        virtual void set_clear_color(glm::vec4 color) noexcept = 0;
        virtual void clear() noexcept = 0;
        virtual void draw_vertex_array(VertexArray&, RenderMode) = 0;
    };

    class RenderBackend {
    public:
        static void set_clear_color(glm::vec4 color) { impl->set_clear_color(color); }
        static void clear() { impl->clear(); }
        static void draw_vertex_array(VertexArray& va, RenderMode mode) { impl->draw_vertex_array(va, mode); }

    private:
        static std::unique_ptr<RenderBackendImpl> impl;
    };
}
