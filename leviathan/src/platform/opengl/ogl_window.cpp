#include "leviathan/lvpch.h"
#include "leviathan/log.h"
#include "leviathan/exc.h"
#include "leviathan/platform/opengl/window.h"
#include "leviathan/platform/opengl/renderer/context.h"

namespace lv {
    namespace opengl {
        constexpr WindowImpl& to_window_ref(void* user_ptr) {
            return *static_cast<WindowImpl*>(user_ptr);
        }

        WindowImpl::WindowImpl(WindowSettings&& settings, EventBus& event_bus) :
            event_bus { event_bus } {
            Log::core_debug("Initialising GLFW with OpenGL {}.{}.", LVGLVersionMajor, LVGLVersionMinor);
            if (!glfwInit()) {
                Log::critical("Failed to initialise GLFW.");
                throw exc::RenderContextException {};
            }

            glfwSetErrorCallback([] (int code, const char* description) {
                Log::core_error("GLFW error {}: {}.", code, description);
            });

            glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, LVGLVersionMajor);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, LVGLVersionMinor);
            glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#if defined(DEBUG)
            glfwWindowHint(GLFW_CONTEXT_DEBUG, GLFW_TRUE);
#endif

            Log::core_debug("Creating GLFW window.");
            handle = glfwCreateWindow(settings.size.x, settings.size.y, settings.title.c_str(), nullptr, nullptr);
            if (!handle) {
                Log::core_critical("Failed to create GLFW window.");
                throw exc::RenderContextException {};
            }

            Log::core_debug("Setting window user pointer.");
            glfwSetWindowUserPointer(handle, this);

            Log::core_debug("Setting window callbacks.");
            glfwSetWindowCloseCallback(handle, [] (GLFWwindow* glfw_window, auto... args) {
                auto& window = to_window_ref(glfwGetWindowUserPointer(glfw_window));
                window.on_window_closed(args...);
            });
            glfwSetFramebufferSizeCallback(handle, [] (GLFWwindow* glfw_window, auto... args) {
                auto& window = to_window_ref(glfwGetWindowUserPointer(glfw_window));
                window.on_window_resized(args...);
            });
            glfwSetKeyCallback(handle, [] (GLFWwindow* glfw_window, auto... args) {
                auto& window = to_window_ref(glfwGetWindowUserPointer(glfw_window));
                window.on_key(args...);
            });
            glfwSetCharCallback(handle, [] (GLFWwindow* glfw_window, auto... args) {
                auto& window = to_window_ref(glfwGetWindowUserPointer(glfw_window));
                window.on_text_entered(args...);
            });
            glfwSetMouseButtonCallback(handle, [] (GLFWwindow* glfw_window, auto... args) {
                auto& window = to_window_ref(glfwGetWindowUserPointer(glfw_window));
                window.on_mouse_button(args...);
            });
            glfwSetCursorPosCallback(handle, [] (GLFWwindow* glfw_window, auto... args) {
                auto& window = to_window_ref(glfwGetWindowUserPointer(glfw_window));
                window.on_mouse_moved(args...);
            });
            glfwSetScrollCallback(handle, [] (GLFWwindow* glfw_window, auto... args) {
                auto& window = to_window_ref(glfwGetWindowUserPointer(glfw_window));
                window.on_mouse_scrolled(args...);
            });

            Log::core_debug("Setting up render context.");
            context = std::make_unique<opengl::Context>(handle);
            if (!context->init()) {
                Log::core_critical("Failed to initialise render context.");
                throw exc::RenderContextException {};
            }
        }

        WindowImpl::~WindowImpl() {
            Log::core_debug("Destroying GLFW window.");
            glfwDestroyWindow(handle);

            Log::core_debug("Terminating GLFW.");
            glfwTerminate();
        }

        void WindowImpl::update() const noexcept {
            glfwPollEvents();
        }

        void WindowImpl::present() const noexcept {
            context->present();
        }

        void WindowImpl::on_window_closed() noexcept {
            event_bus.emplace(window_closed_event());
        }

        void WindowImpl::on_window_resized(int width, int height) noexcept {
            event_bus.emplace(window_resized_event(width, height));
        }

        void WindowImpl::on_key(int key, int scancode, int action, int mods) noexcept {
            auto shift = (mods & GLFW_MOD_SHIFT) > 0;
            auto control = (mods & GLFW_MOD_CONTROL) > 0;
            auto alt = (mods & GLFW_MOD_ALT) > 0;
            auto super = (mods & GLFW_MOD_SUPER) > 0;
            switch (action) {
                case GLFW_PRESS: event_bus.emplace(key_pressed_event((KeyCode) key, shift, control, alt, super)); return;
                case GLFW_RELEASE: event_bus.emplace(key_released_event((KeyCode) key, shift, control, alt, super)); return;
                case GLFW_REPEAT: return;
                default: Log::core_error("Unknown GLFW key action: {}.", action); return;
            }
        }

        void WindowImpl::on_text_entered(unsigned int codepoint) noexcept {
            event_bus.emplace(text_entered_event(codepoint));
        }

        void WindowImpl::on_mouse_button(int button, int action, int mods) noexcept {
            switch (action) {
                case GLFW_PRESS: event_bus.emplace(button_pressed_event((ButtonCode) button)); return;
                case GLFW_RELEASE: event_bus.emplace(button_released_event((ButtonCode) button)); return;
                default: Log::core_error("Unknown GLFW mouse button action: {}.", action); return;
            }
        }

        void WindowImpl::on_mouse_moved(double x, double y) noexcept {
            event_bus.emplace(mouse_moved_event({ static_cast<float>(x), static_cast<float>(y) }));
        }

        void WindowImpl::on_mouse_scrolled(double x_offset, double y_offset) noexcept {
            event_bus.emplace(mouse_scrolled_event({ static_cast<float>(x_offset), static_cast<float>(y_offset) }));
        }
    }
}