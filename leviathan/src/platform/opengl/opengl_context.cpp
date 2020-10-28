#include "leviathan/lvpch.h"

#include "leviathan/log.h"
#include "leviathan/platform/opengl/opengl_context.h"

namespace lv {
    OpenGLContext::OpenGLContext(GLFWwindow* handle) noexcept : handle { handle } {}

    bool OpenGLContext::init() noexcept {
        make_current();

        Log::core_debug("Loading OpenGL core profile function into GLFW loadproc");
        if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
            Log::core_error("Failed to load OpenGL core profile functions via GLAD");
            return false;
        }
        Log::core_info("OpenGL version {}.{} has been initialised", GLVersion.major, GLVersion.minor);

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

        return true;
    }

    void OpenGLContext::make_current() noexcept {
        glfwMakeContextCurrent(handle);
    }

    void OpenGLContext::present() noexcept {
        glfwSwapBuffers(handle);
    }
}