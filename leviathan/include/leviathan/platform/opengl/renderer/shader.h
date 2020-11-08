#pragma once

#include "leviathan/lvpch.h"
#include "leviathan/platform/opengl/opengl.h"
#include "leviathan/renderer/shader.h"

namespace lv {
    namespace opengl {
        class Shader : public lv::Shader {
        public:
            explicit Shader(std::string const& filename, std::unordered_set<Type> const& types);

            void use() override;

            void set_mat4(std::string const& name, glm::mat4) override;
            void set_vec4(std::string const& name, glm::vec4) override;
            void set_int(std::string const& name, int32_t) override;
            void set_float(std::string const& name, float) override;

            ~Shader() override;

        private:
            void build_uniform_cache();
            bool Shader::get_uniform(std::string const& name, GLenum type, std::pair<GLint, GLenum>& uniform);

        private:
            GLuint program;

            std::unordered_map<std::string, std::pair<GLint, GLenum>> uniforms;
        };
    }
}
