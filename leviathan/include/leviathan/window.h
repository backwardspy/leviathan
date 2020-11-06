#pragma once

#include "leviathan/lvpch.h"
#include "leviathan/events.h"
#include "leviathan/renderer/context.h"
#include "leviathan/core/noncopyable.h"

namespace lv {
    struct WindowSettings {
        glm::ivec2 size { 800, 600 };
        std::string title { "Leviathan Application" };
    };

    class WindowImpl {
    public:
        virtual void update() const = 0;
        virtual void present() const = 0;

        virtual Context& get_context() const = 0;
        virtual glm::ivec2 get_size() const = 0;
        virtual float get_aspect_ratio() const = 0;
        virtual glm::vec2 get_mouse_position() const = 0;
        virtual std::any get_native_handle() const = 0;

        virtual ~WindowImpl() = default;
    };

    class Window : public NonCopyable {
    public:
        Window(WindowSettings&&, EventBus&);

        void update() const { impl->update(); }
        void present() const { impl->present(); }

        glm::ivec2 get_size() const { return impl->get_size(); }
        virtual float get_aspect_ratio() const { return impl->get_aspect_ratio(); }
        Context& get_context() const { return impl->get_context(); }
        glm::vec2 get_mouse_position() const { return impl->get_mouse_position(); }
        std::any get_native_handle() const { return impl->get_native_handle(); }

    private:
        scope<WindowImpl> impl;
    };
}
