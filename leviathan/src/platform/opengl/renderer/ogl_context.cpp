#include "leviathan/lvpch.h"

#include "leviathan/log.h"
#include "leviathan/exc.h"
#include "leviathan/platform/opengl/renderer/context.h"
#include "leviathan/platform/opengl/renderer/shader.h"
#include "leviathan/platform/opengl/renderer/vertex_array.h"

namespace lv {
    namespace opengl {
        Context::Context(GLFWwindow* handle) : handle { handle } {}

#if defined(DEBUG)
        char const* const gl_debug_source_name(GLenum source) {
            switch (source) {
                case GL_DEBUG_SOURCE_API: return "API";
                case GL_DEBUG_SOURCE_APPLICATION: return "Application";
                case GL_DEBUG_SOURCE_OTHER: return "Other";
                case GL_DEBUG_SOURCE_SHADER_COMPILER: return "Shader compiler";
                case GL_DEBUG_SOURCE_THIRD_PARTY: return "Third party";
                case GL_DEBUG_SOURCE_WINDOW_SYSTEM: return "Window system";
                default: return "Unknown OpenGL debug source";
            }
        }

        char const* const gl_debug_type_name(GLenum type) {
            switch (type) {
                case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: return "Deprecated behaviour";
                case GL_DEBUG_TYPE_ERROR: return "Error";
                case GL_DEBUG_TYPE_MARKER: return "Marker";
                default: return "Unknown OpenGL debug type";
            }
        }

        void APIENTRY gl_debug_msg_callback(
            GLenum source,
            GLenum type,
            GLuint id,
            GLenum severity,
            GLsizei length,
            GLchar const* const message,
            void const* const user_param
        ) {
            Log::Level level;
            switch (severity) {
                case GL_DEBUG_SEVERITY_NOTIFICATION: level = Log::Level::Info; break;
                case GL_DEBUG_SEVERITY_LOW: level = Log::Level::Warn; break;
                case GL_DEBUG_SEVERITY_MEDIUM: level = Log::Level::Error; break;
                case GL_DEBUG_SEVERITY_HIGH: level = Log::Level::Critical; break;
                default:
                    Log::core_warn("Unhandled GL message severity: {}. Defaulting to critical severity.", severity);
                    level = Log::Level::Critical;
                    break;
            }
            Log::core_log(level, "OpenGL debug message:");
            Log::core_log(level, "* Source: {}", gl_debug_source_name(source));
            Log::core_log(level, "* Type: {}", gl_debug_type_name(type));
            Log::core_log(level, "* Message: {}", message);
        }
#endif

        bool Context::init() {
            make_current();

            Log::core_debug("Loading OpenGL core profile function into GLFW loadproc.");
            if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
                Log::core_error("Failed to load OpenGL core profile functions via GLAD.");
                return false;
            }
            Log::core_info("OpenGL version {}.{} has been initialised.", GLVersion.major, GLVersion.minor);

            if (GLVersion.major != LVGLVersionMajor || GLVersion.minor != LVGLVersionMinor) {
                Log::core_warn(
                    "Warning, loaded OpenGL version does not match engine configuration. "
                    "Expected {}.{}, got {}.{}. "
                    "This might indicate a mismatched glad version.",
                    LVGLVersionMajor, LVGLVersionMinor,
                    GLVersion.major, GLVersion.minor
                );
            }

            Log::core_debug("OpenGL driver strings:");
            Log::core_debug("* Vendor: {}", glGetString(GL_VENDOR));
            Log::core_debug("* Renderer: {}", glGetString(GL_RENDERER));
            Log::core_debug("* Version: {}", glGetString(GL_VERSION));
            Log::core_debug("* GLSL Version: {}", glGetString(GL_SHADING_LANGUAGE_VERSION));

#if defined(DEBUG)
            Log::core_debug("Enabling debug output.");
            glEnable(GL_DEBUG_OUTPUT);
            glDebugMessageCallback(gl_debug_msg_callback, nullptr);
#endif

            return true;
        }

        void Context::make_current() {
            glfwMakeContextCurrent(handle);
        }

        void Context::present() {
            glfwSwapBuffers(handle);
        }

        std::shared_ptr<lv::Shader> Context::make_shader(Shader::SourceMap const& sources) {
            return std::make_unique<lv::opengl::Shader>(sources);
        }

        std::shared_ptr<lv::VertexArray> Context::make_vertex_array(std::vector<Vertex> const&& vertices) {
            return std::make_unique<lv::opengl::VertexArray>(std::move(vertices), auto_index(vertices.size()));
        }

        std::shared_ptr<lv::VertexArray> Context::make_vertex_array(std::vector<Vertex> const&& vertices, std::vector<Index> const&& indices) {
            return std::make_unique<lv::opengl::VertexArray>(std::move(vertices), std::move(indices));
        }
    }
}