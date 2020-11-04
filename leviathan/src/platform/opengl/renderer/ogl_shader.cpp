#include "leviathan/lvpch.h"

#include <glm/gtc/type_ptr.hpp>

#include "leviathan/exc.h"
#include "leviathan/log.h"
#include "leviathan/platform/opengl/renderer/shader.h"

namespace lv {
    namespace opengl {
        GLuint compile_shader(GLenum type, std::string const& source) {
            auto shader = glCreateShader(type);
            char const* const sources[] = { source.c_str() };
            GLint const source_lengths[] = { (GLint) source.length() };
            glShaderSource(shader, 1, sources, source_lengths);
            glCompileShader(shader);
            return shader;
        }

        constexpr GLenum to_gl_shader_type(lv::Shader::Type type) {
            switch (type) {
                case lv::Shader::Type::Pixel: return GL_FRAGMENT_SHADER;
                case lv::Shader::Type::Vertex: return GL_VERTEX_SHADER;
                default: throw exc::EnumeratorNotImplementedError {};
            }
        }

        Shader::Shader(lv::Shader::SourceMap const& sources) :
            program { glCreateProgram() } {
            std::vector<GLuint> shaders;
            for (auto& source : sources) {
                shaders.emplace_back(compile_shader(to_gl_shader_type(source.first), source.second));
                glAttachShader(program, shaders.back());
            }

            glLinkProgram(program);

            for (auto shader : shaders) {
                glDetachShader(program, shader);
                glDeleteShader(shader);
            }

            build_uniform_cache();
        }

        Shader::~Shader() {
            glDeleteProgram(program);
            program = 0;
        }

        void Shader::use() {
            glUseProgram(program);
        }

        bool Shader::get_uniform(std::string const& name, GLenum type, std::pair<GLint, GLenum>& uniform) {
            use();
            auto it = uniforms.find(name);
            if (it == std::end(uniforms)) {
                Log::core_error("get_uniform: uniform \"{}\" could not be found.", name);
                return false;
            }
            uniform = it->second;
            if (uniform.second != type) {
                Log::core_warn("get_uniform: uniform \"{}\" is not the expected type.", name);
            }
            return true;
        }

        void Shader::set_mat4(std::string const& name, glm::mat4 mat) {
            std::pair<GLint, GLenum> uniform;
            if (!get_uniform(name, GL_FLOAT_MAT4, uniform)) return;
            glUniformMatrix4fv(uniform.first, 1, GL_FALSE, glm::value_ptr(mat));
        }

        void Shader::set_vec4(std::string const& name, glm::vec4 vec) {
            std::pair<GLint, GLenum> uniform;
            if (!get_uniform(name, GL_FLOAT_VEC4, uniform)) return;
            glUniform4fv(uniform.first, 1, glm::value_ptr(vec));
        }

        void Shader::build_uniform_cache() {
            GLint active_uniforms;
            glGetProgramInterfaceiv(program, GL_UNIFORM, GL_ACTIVE_RESOURCES, &active_uniforms);

            GLenum const props[] { GL_NAME_LENGTH, GL_LOCATION, GL_TYPE };
            auto prop_count = std::size(props);
            std::vector<GLint> values(prop_count);

            std::vector<GLchar> name_buffer(32);
            for (auto idx = 0; idx < active_uniforms; ++idx) {
                glGetProgramResourceiv(
                    program,
                    GL_UNIFORM,
                    idx,
                    static_cast<GLsizei>(prop_count),
                    props,
                    static_cast<GLsizei>(prop_count),
                    nullptr,
                    values.data()
                );

                name_buffer.resize(values[0]); // GL_NAME_LENGTH
                auto length = std::size(name_buffer);

                glGetProgramResourceName(
                    program,
                    GL_UNIFORM,
                    idx,
                    static_cast<GLsizei>(length),
                    nullptr,
                    name_buffer.data()
                );

                std::string name { name_buffer.data(), length - 1 };
                auto loc = values[1];
                auto type = values[2];

                uniforms[name] = std::make_pair(loc, type);
            }

            Log::core_debug("Cached {} uniform(s):", std::size(uniforms));
            for (auto& pair : uniforms) {
                Log::core_debug("* {}: {}", pair.first, pair.second.first);
            }
        }
    }
}