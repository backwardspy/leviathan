#pragma once

#include "leviathan/platform/opengl/opengl.h"
#include "leviathan/window.h"
#include "leviathan/renderer/context.h"

namespace lv {
    namespace opengl {
        class WindowImpl : public lv::WindowImpl {
        public:
            WindowImpl(WindowSettings&&, EventBus&);

            virtual void update() const noexcept override;
            virtual void present() const noexcept override;

            virtual Context& get_context() const noexcept override { return *context; }
            virtual glm::ivec2 get_size() const noexcept override { return settings.size; }
            virtual std::any get_native_handle() const noexcept override { return handle; }

            ~WindowImpl();

        private:
            void on_window_closed() noexcept;
            void on_window_resized(int width, int height) noexcept;
            void on_key(int key, int scancode, int action, int mods) noexcept;
            void on_text_entered(unsigned int codepoint) noexcept;
            void on_mouse_button(int button, int action, int mods) noexcept;
            void on_mouse_moved(double x, double y) noexcept;
            void on_mouse_scrolled(double x_offset, double y_offset) noexcept;

        private:
            std::unique_ptr<Context> context;
            WindowSettings settings;
            EventBus& event_bus;
            GLFWwindow* handle;
        };
    }
}
