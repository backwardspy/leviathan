#pragma once

#include <GLFW/glfw3.h>

#include "leviathan/renderer/render_context.h"

namespace lv {
    class OpenGLContext : public RenderContext {
    public:
        OpenGLContext(GLFWwindow* handle) noexcept;

        virtual bool init() noexcept override;
        virtual void make_current() noexcept override;
        virtual void present() noexcept override;

    private:
        GLFWwindow* handle;
    };
}
