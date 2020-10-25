#include "leviathan/lvpch.h"

#include "leviathan/window.h"
#include "leviathan/log.h"

namespace lv {
    Window::Window(WindowSettings&& settings, EventBus& event_bus) noexcept :
        settings { std::move(settings) },
        event_bus { event_bus },
        handle { nullptr }
    {}

    Window::~Window() noexcept {
        Log::core_debug("Destroying GLFW window");
        glfwDestroyWindow(handle);

        Log::core_debug("Terminating GLFW");
        glfwTerminate();
    }

    bool Window::create() noexcept {
        Log::core_debug("Initialising GLFW with OpenGL {}.{}", LVGLVersionMajor, LVGLVersionMinor);
        if (!glfwInit()) {
            Log::critical("Failed to initialise GLFW.");
            return false;
        }

        glfwSetErrorCallback([] (int code, const char* description) {
            Log::core_error("GLFW error {}: {}", code, description);
        });

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, LVGLVersionMajor);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, LVGLVersionMinor);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        Log::core_debug("Creating GLFW window");
        handle = glfwCreateWindow(settings.width, settings.height, settings.title.c_str(), nullptr, nullptr);
        if (!handle) {
            Log::core_critical("Failed to create GLFW window.");
            return false;
        }

        Log::core_debug("Setting window user pointer");
        glfwSetWindowUserPointer(handle, this);

        Log::core_debug("Setting window callbacks");
        glfwSetWindowCloseCallback(handle, [] (GLFWwindow* glfw_window, auto... args) {
            auto& window = *(Window*) glfwGetWindowUserPointer(glfw_window);
            window.on_window_closed(args...);
        });
        glfwSetFramebufferSizeCallback(handle, [] (GLFWwindow* glfw_window, auto... args) {
            auto& window = *(Window*) glfwGetWindowUserPointer(glfw_window);
            window.on_window_resized(args...);
        });
        glfwSetKeyCallback(handle, [] (GLFWwindow* glfw_window, auto... args) {
            auto& window = *(Window*) glfwGetWindowUserPointer(glfw_window);
            window.on_key(args...);
        });
        glfwSetCharCallback(handle, [] (GLFWwindow* glfw_window, auto... args) {
            auto& window = *(Window*) glfwGetWindowUserPointer(glfw_window);
            window.on_text_entered(args...);
        });
        glfwSetMouseButtonCallback(handle, [] (GLFWwindow* glfw_window, auto... args) {
            auto& window = *(Window*) glfwGetWindowUserPointer(glfw_window);
            window.on_mouse_button(args...);
        });
        glfwSetCursorPosCallback(handle, [] (GLFWwindow* glfw_window, auto... args) {
            auto& window = *(Window*) glfwGetWindowUserPointer(glfw_window);
            window.on_mouse_moved(args...);
        });
        glfwSetScrollCallback(handle, [] (GLFWwindow* glfw_window, auto... args) {
            auto& window = *(Window*) glfwGetWindowUserPointer(glfw_window);
            window.on_mouse_scrolled(args...);
        });

        Log::core_debug("Making window context current");
        glfwMakeContextCurrent(handle);

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

        return true;
    }

    void Window::update() const noexcept {
        glfwPollEvents();
    }

    void Window::clear() const noexcept {
        glClearColor(0.3f, 0.05f, 0.8f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
    }

    void Window::display() const noexcept {
        glfwSwapBuffers(handle);
    }

    void Window::on_window_closed() noexcept {
        event_bus.emplace(window_closed_event());
    }

    void Window::on_window_resized(int width, int height) noexcept {
        event_bus.emplace(window_resized_event(width, height));
    }

    void Window::on_key(int key, int scancode, int action, int mods) noexcept {
        auto shift = (mods & GLFW_MOD_SHIFT) > 0;
        auto control = (mods & GLFW_MOD_CONTROL) > 0;
        auto alt = (mods & GLFW_MOD_ALT) > 0;
        auto super = (mods & GLFW_MOD_SUPER) > 0;
        switch (action) {
            case GLFW_PRESS: event_bus.emplace(key_pressed_event((KeyCode) key, shift, control, alt, super)); return;
            case GLFW_RELEASE: event_bus.emplace(key_released_event((KeyCode) key, shift, control, alt, super)); return;
            case GLFW_REPEAT: return;
            default: Log::core_error("Unknown GLFW key action: {}", action); return;
        }
    }

    void Window::on_text_entered(unsigned int codepoint) noexcept {
        event_bus.emplace(text_entered_event(codepoint));
    }

    void Window::on_mouse_button(int button, int action, int mods) noexcept {
        switch (action) {
            case GLFW_PRESS: event_bus.emplace(button_pressed_event((ButtonCode) button)); return;
            case GLFW_RELEASE: event_bus.emplace(button_released_event((ButtonCode) button)); return;
            default: Log::core_error("Unknown GLFW mouse button action: {}", action); return;
        }
    }

    void Window::on_mouse_moved(double x, double y) noexcept {
        event_bus.emplace(mouse_moved_event(x, y));
    }

    void Window::on_mouse_scrolled(double x_offset, double y_offset) noexcept {
        event_bus.emplace(mouse_scrolled_event(x_offset, y_offset));
    }
}