#include "leviathan/lvpch.h"

#include "leviathan/exc.h"
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

        Shader::Shader(const lv::Shader::SourceMap& sources) {
            std::vector<GLuint> shaders;
            program = glCreateProgram();
            for (auto& source : sources) {
                shaders.emplace_back(compile_shader(to_gl_shader_type(source.first), source.second));
                glAttachShader(program, shaders.back());
            }

            glLinkProgram(program);

            for (auto shader : shaders) {
                glDetachShader(program, shader);
                glDeleteShader(shader);
            }
        }

        Shader::~Shader() noexcept {
            glDeleteProgram(program);
            program = 0;
        }

        void Shader::use() noexcept {
            glUseProgram(program);
        }
    }
}