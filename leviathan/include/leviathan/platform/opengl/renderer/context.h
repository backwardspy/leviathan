#pragma once

#include "leviathan/platform/opengl/opengl.h"
#include "leviathan/renderer/context.h"

namespace lv {
    namespace opengl {
        class Context : public lv::Context {
        public:
            explicit Context(GLFWwindow* handle);

            bool init() override;
            void make_current() override;
            void present() override;

            std::shared_ptr<lv::Shader> make_shader(Shader::SourceMap const&) override;
            std::shared_ptr<lv::VertexArray> make_vertex_array(std::vector<Vertex> const&& vertices) override;
            std::shared_ptr<lv::VertexArray> make_vertex_array(std::vector<Vertex> const&& vertices, std::vector<Index> const&& indices) override;

        private:
            GLFWwindow* handle;
        };
    }
}
