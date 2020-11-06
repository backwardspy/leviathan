#pragma once

#include "leviathan/core.h"
#include "leviathan/platform/opengl/opengl.h"
#include "leviathan/window.h"
#include "leviathan/renderer/context.h"

namespace lv {
    namespace opengl {
        class WindowImpl : public lv::WindowImpl {
        public:
            WindowImpl(WindowSettings&&, EventBus&);

            void update() const override;
            void present() const override;

            Context& get_context() const override { return *context; }
            glm::ivec2 get_size() const override;
            float get_aspect_ratio() const override;
            glm::vec2 get_mouse_position() const override;
            std::any get_native_handle() const override { return handle; }

            ~WindowImpl() override;

        private:
            void on_window_closed();
            void on_window_resized(int width, int height);
            void on_key(int key, int scancode, int action, int mods);
            void on_text_entered(unsigned int codepoint);
            void on_mouse_button(int button, int action, int mods);
            void on_mouse_moved(double x, double y);
            void on_mouse_scrolled(double x_offset, double y_offset);

        private:
            scope<Context> context;
            EventBus& event_bus;
            GLFWwindow* handle;
        };
    }
}
