#pragma once

#include "leviathan/lvpch.h"
#include "leviathan/platform/opengl/opengl.h"
#include "leviathan/renderer/shader.h"

namespace lv {
    namespace opengl {
        class Shader : public lv::Shader {
        public:
            explicit Shader(const lv::Shader::SourceMap& sources);

            void use() noexcept override;

            void set_mat4(const std::string& name, glm::mat4 mat) override;

            ~Shader() noexcept override;

        private:
            void build_uniform_cache() noexcept;
            bool Shader::get_uniform(const std::string& name, GLenum type, std::pair<GLint, GLenum>& uniform);

        private:
            GLuint program;

            std::unordered_map<std::string, std::pair<GLint, GLenum>> uniforms;
        };
    }
}
