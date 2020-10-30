#pragma once

#include "leviathan/platform/opengl/opengl.h"
#include "leviathan/renderer/context.h"

namespace lv {
    namespace opengl {
        class Context : public lv::Context {
        public:
            Context(GLFWwindow* handle) noexcept;

            virtual bool init() noexcept override;
            virtual void make_current() noexcept override;
            virtual void present() noexcept override;

            virtual std::unique_ptr<lv::Shader> create_shader(Shader::SourceMap sources) noexcept override;
            virtual std::unique_ptr<lv::VertexArray> create_vertex_array(std::vector<Vertex>&& vertices) noexcept override;
            virtual std::unique_ptr<lv::VertexArray> create_vertex_array(std::vector<Vertex>&& vertices, std::vector<Index>&& indices) noexcept override;

        private:
            GLFWwindow* handle;
        };
    }
}