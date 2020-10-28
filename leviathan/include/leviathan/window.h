#pragma once

#include "lvpch.h"

#include "events.h"
#include "renderer/render_context.h"

namespace lv {
    struct WindowSettings {
        int width, height;
        std::string title;
        WindowSettings(int width, int height, std::string&& title) noexcept :
            width { width },
            height { height },
            title { std::move(title) }
        {}
    };

    class Window {
    public:
        Window(WindowSettings&&, EventBus&) noexcept;

        Window(const Window&) = delete;
        void operator=(const Window&) = delete;

        bool create() noexcept;
        void update() const noexcept;
        void clear() const noexcept;
        void display() const noexcept;

        constexpr int get_width() const noexcept { return settings.width; }
        constexpr int get_height() const noexcept { return settings.height; }
        constexpr GLFWwindow* get_glfw_handle() const noexcept { return handle; }

        ~Window() noexcept;

    private:
        void on_window_closed() noexcept;
        void on_window_resized(int width, int height) noexcept;
        void on_key(int key, int scancode, int action, int mods) noexcept;
        void on_text_entered(unsigned int codepoint) noexcept;
        void on_mouse_button(int button, int action, int mods) noexcept;
        void on_mouse_moved(double x, double y) noexcept;
        void on_mouse_scrolled(double x_offset, double y_offset) noexcept;

        WindowSettings settings;
        GLFWwindow* handle;
        EventBus& event_bus;
        std::unique_ptr<RenderContext> context;
    };
}
