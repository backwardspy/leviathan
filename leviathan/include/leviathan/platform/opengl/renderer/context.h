#pragma once

#include "leviathan/platform/opengl/opengl.h"
#include "leviathan/renderer/context.h"

namespace lv {
    namespace opengl {
        class Context : public lv::Context {
        public:
            explicit Context(GLFWwindow* handle) noexcept;

            bool init() noexcept override;
            void make_current() noexcept override;
            void present() noexcept override;

            std::unique_ptr<lv::Shader> create_shader(Shader::SourceMap sources) noexcept override;
            std::unique_ptr<lv::VertexArray> create_vertex_array(std::vector<Vertex>&& vertices) noexcept override;
            std::unique_ptr<lv::VertexArray> create_vertex_array(std::vector<Vertex>&& vertices, std::vector<Index>&& indices) noexcept override;

        private:
            GLFWwindow* handle;
        };
    }
}
