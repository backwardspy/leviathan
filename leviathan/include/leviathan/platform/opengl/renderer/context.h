#pragma once

#include "leviathan/core.h"
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

            ref<lv::Shader> make_shader(std::string const&) override;
            ref<lv::VertexArray> make_vertex_array(std::vector<Vertex>&& vertices) override;
            ref<lv::VertexArray> make_vertex_array(std::vector<Vertex>&& vertices, std::vector<Index>&& indices) override;
            ref<lv::Texture> make_texture(std::string const& filename) override;

        private:
            GLFWwindow* handle;
        };
    }
}
