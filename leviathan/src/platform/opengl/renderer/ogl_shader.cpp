#include "leviathan/lvpch.h"
#include "leviathan/platform/opengl/renderer/shader.h"

namespace lv {
    namespace opengl {
        std::string shader_type_definition_line(GLenum type) {
            switch (type) {
                case GL_FRAGMENT_SHADER: return "#define FRAGMENT_SHADER\n";
                case GL_VERTEX_SHADER: return "#define VERTEX_SHADER\n";
                default: throw exc::EnumeratorNotImplemented {};
            }
        }

        GLuint compile_shader(GLenum type, std::string const& source) {
            auto shader = glCreateShader(type);

            std::array<std::string, 3> sources = {
                "#version 460 core\n",
                shader_type_definition_line(type),
                source,
            };

            std::array<char const*, sources.size()> source_ptrs;
            std::transform(
                std::begin(sources), std::end(sources),
                std::begin(source_ptrs),
                [] (std::string const& source) { return source.c_str(); }
            );

            std::array<GLint, sources.size()> source_lengths;
            std::transform(
                std::begin(sources), std::end(sources),
                std::begin(source_lengths),
                [] (std::string const& source) { return static_cast<GLint>(source.length()); }
            );

            glShaderSource(shader, static_cast<GLsizei>(sources.size()), source_ptrs.data(), source_lengths.data());
            glCompileShader(shader);
            return shader;
        }

        Shader::Shader(std::string const& filename) :
            program { glCreateProgram() } {
            std::ifstream shader_file(filename);
            if (!shader_file) {
                Log::core_error("Failed to load shader file {}", filename);
                throw exc::FileReadError {};
            }

            std::string source;
            shader_file.seekg(0, std::ios::end);
            source.resize(shader_file.tellg());
            shader_file.seekg(0, std::ios::beg);
            shader_file.read(source.data(), source.size());
            shader_file.close();

            // TODO: specify which shader types are in the source file
            std::vector<GLuint> shaders = {
                compile_shader(GL_VERTEX_SHADER, source),
                compile_shader(GL_FRAGMENT_SHADER, source),
            };

            for (auto const& shader : shaders) glAttachShader(program, shader);
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
            if (options.alpha_blend) {
                glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            } else {
                glBlendFunc(GL_ONE, GL_ZERO);
            }
        }

        bool Shader::get_uniform(std::string const& name, GLenum type, std::pair<GLint, GLenum>& uniform) {
            use();
            auto it = uniforms.find(name);
            if (it == std::end(uniforms)) {
                Log::core_error("get_uniform: uniform \"{}\" could not be found.", name);
                return false;
            }
            uniform = it->second;
            if (uniform.second != type && !(uniform.second == GL_SAMPLER_2D && type == GL_INT)) {
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

        void Shader::set_int(std::string const& name, int32_t value) {
            std::pair<GLint, GLenum> uniform;
            if (!get_uniform(name, GL_INT, uniform)) return;
            glUniform1i(uniform.first, value);
        }

        void Shader::set_float(std::string const& name, float value) {
            std::pair<GLint, GLenum> uniform;
            if (!get_uniform(name, GL_FLOAT, uniform)) return;
            glUniform1f(uniform.first, value);
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
                std::string type_name;
                switch (pair.second.second) {
                    case GL_FLOAT_MAT4: type_name = "mat4"; break;
                    case GL_FLOAT_VEC4: type_name = "vec4"; break;
                    case GL_INT: type_name = "int"; break;
                    case GL_UNSIGNED_INT: type_name = "uint"; break;
                    case GL_SAMPLER_2D: type_name = "sampler2D"; break;
                    default: type_name = "unknown type"; break;
                }
                Log::core_debug("* {} {}: {}", type_name, pair.first, pair.second.first);
            }
        }
    }
}