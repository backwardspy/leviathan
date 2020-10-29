#include "leviathan/lvpch.h"

#include "leviathan/log.h"
#include "leviathan/exc.h"
#include "leviathan/platform/opengl/opengl_context.h"
#include "leviathan/platform/opengl/opengl_shader.h"
#include "leviathan/platform/opengl/opengl_buffer.h"

namespace lv {
    OpenGLContext::OpenGLContext(GLFWwindow* handle) noexcept : handle { handle } {}

#if defined(DEBUG)
    const char* gl_debug_source_name(GLenum source) {
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

    const char* gl_debug_type_name(GLenum type) {
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
        const GLchar* message,
        const void* user_param
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

    bool OpenGLContext::init() noexcept {
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
        Log::core_debug("*  Vendor: {}", glGetString(GL_VENDOR));
        Log::core_debug("*  Renderer: {}", glGetString(GL_RENDERER));
        Log::core_debug("*  Version: {}", glGetString(GL_VERSION));
        Log::core_debug("*  GLSL Version: {}", glGetString(GL_SHADING_LANGUAGE_VERSION));

#if defined(DEBUG)
        Log::core_debug("Enabling debug output.");
        glEnable(GL_DEBUG_OUTPUT);
        glDebugMessageCallback(gl_debug_msg_callback, nullptr);
#endif

        return true;
    }

    void OpenGLContext::make_current() noexcept {
        glfwMakeContextCurrent(handle);
    }

    void OpenGLContext::present() noexcept {
        glfwSwapBuffers(handle);
    }

#pragma region Resource Creation

    std::unique_ptr<Shader> OpenGLContext::create_shader(Shader::SourceMap sources) noexcept {
        return std::make_unique<lv::opengl::Shader>(sources);
    }

    std::unique_ptr<VertexBuffer> OpenGLContext::create_vertex_buffer(std::vector<Vertex>&& vertices) noexcept {
        return std::make_unique<lv::opengl::VertexBuffer>(std::move(vertices));
    }

    std::unique_ptr<IndexBuffer> OpenGLContext::create_index_buffer(std::vector<Index>&& indices) noexcept {
        return std::make_unique<lv::opengl::IndexBuffer>(std::move(indices));
    }

#pragma endregion

#pragma region Rendering

    constexpr GLenum to_gl_mode(RenderMode mode) {
        switch (mode) {
            case RenderMode::Triangles: return GL_TRIANGLES;
            default: throw exc::EnumeratorNotImplementedError {};
        }
    }

    void OpenGLContext::draw_indexed(RenderMode mode, VertexBuffer& vertices, IndexBuffer& indices, Shader& shader) noexcept {
        vertices.use();
        indices.use();
        shader.use();
        glDrawElements(to_gl_mode(mode), (GLsizei) indices.get_element_count(), GL_UNSIGNED_INT, nullptr);
    }

#pragma endregion
}