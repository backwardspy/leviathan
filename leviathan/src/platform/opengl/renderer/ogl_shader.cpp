#include "leviathan/lvpch.h"

#include <glm/gtc/type_ptr.hpp>

#include "leviathan/exc.h"
#include "leviathan/log.h"
#include "leviathan/platform/opengl/renderer/shader.h"

namespace lv {
    namespace opengl {
        GLuint compile_shader(GLenum type, const std::string& source) noexcept {
            auto shader = glCreateShader(type);
            const char* sources[] = { source.c_str() };
            const GLint source_lengths[] = { (GLint) source.length() };
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

        Shader::Shader(const lv::Shader::SourceMap& sources) :
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

        Shader::~Shader() noexcept {
            glDeleteProgram(program);
            program = 0;
        }

        void Shader::use() noexcept {
            glUseProgram(program);
        }

        bool Shader::get_uniform(const std::string& name, GLenum type, std::pair<GLint, GLenum>& uniform) {
            use();
            auto it = uniforms.find(name);
            if (it == std::end(uniforms)) {
                Log::error("get_uniform: uniform \"{}\" could not be found.", name);
                return false;
            }
            uniform = it->second;
            if (uniform.second != type) {
                Log::warn("get_uniform: uniform \"{}\" is not the expected type.", name);
            }
            return true;
        }

        void Shader::set_mat4(const std::string& name, glm::mat4 mat) {
            std::pair<GLint, GLenum> uniform;
            if (!get_uniform(name, GL_FLOAT_MAT4, uniform)) return;
            glUniformMatrix4fv(uniform.first, 1, GL_FALSE, glm::value_ptr(mat));
        }

        void Shader::build_uniform_cache() noexcept {
            GLint active_uniforms;
            glGetProgramInterfaceiv(program, GL_UNIFORM, GL_ACTIVE_RESOURCES, &active_uniforms);

            const GLenum props[] { GL_NAME_LENGTH, GL_LOCATION, GL_TYPE };
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

            Log::debug("Cached {} uniform(s):", std::size(uniforms));
            for (auto& pair : uniforms) {
                Log::debug("* {}: {}", pair.first, pair.second.first);
            }
        }
    }
}