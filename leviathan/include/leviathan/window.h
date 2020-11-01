#pragma once

#include "leviathan/lvpch.h"
#include "leviathan/events.h"
#include "leviathan/renderer/context.h"

namespace lv {
    struct WindowSettings {
        glm::ivec2 size { 800, 600 };
        std::string title { "Leviathan Application" };
    };

    class WindowImpl {
    public:
        virtual void update() const noexcept = 0;
        virtual void present() const noexcept = 0;

        virtual Context& get_context() const noexcept = 0;
        virtual glm::ivec2 get_size() const noexcept = 0;
        virtual float get_aspect_ratio() const noexcept = 0;
        virtual glm::vec2 get_mouse_position() const noexcept = 0;
        virtual std::any get_native_handle() const noexcept = 0;

        virtual ~WindowImpl() noexcept {}
    };

    class Window {
    public:
        Window(WindowSettings&&, EventBus&);

        Window(const Window&) = delete;
        void operator=(const Window&) = delete;

        void update() const noexcept { impl->update(); }
        void present() const noexcept { impl->present(); }

        glm::ivec2 get_size() const noexcept { return impl->get_size(); }
        virtual float get_aspect_ratio() const noexcept { return impl->get_aspect_ratio(); }
        Context& get_context() const noexcept { return impl->get_context(); }
        glm::vec2 get_mouse_position() const { return impl->get_mouse_position(); }
        std::any get_native_handle() const noexcept { return impl->get_native_handle(); }

    private:
        std::unique_ptr<WindowImpl> impl;
    };
}
