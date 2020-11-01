#pragma once

#include "leviathan/platform/opengl/opengl.h"
#include "leviathan/window.h"
#include "leviathan/renderer/context.h"

namespace lv {
    namespace opengl {
        class WindowImpl : public lv::WindowImpl {
        public:
            WindowImpl(WindowSettings&&, EventBus&);

            void update() const noexcept override;
            void present() const noexcept override;

            Context& get_context() const noexcept override { return *context; }
            glm::ivec2 get_size() const noexcept override;
            float get_aspect_ratio() const noexcept override;
            glm::vec2 get_mouse_position() const noexcept override;
            std::any get_native_handle() const noexcept override { return handle; }

            ~WindowImpl() override;

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
            EventBus& event_bus;
            GLFWwindow* handle;
        };
    }
}
