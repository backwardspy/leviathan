#pragma once

#include "lvpch.h"
#include "events.h"
#include "input.h"
#include "layer_stack.h"
#include "window.h"
#include "core/noncopyable.h"

namespace lv {
    class Application : public IEventListener, public NonCopyable {
    public:
        int run();
        void stop();

        constexpr Window const& get_window() const { return window; }
        constexpr Window& get_window() { return window; }  // cppcheck-suppress functionConst

        Context const& get_render_context() const { return window.get_context(); }
        Context& get_render_context() { return window.get_context(); }  // cppcheck-suppress functionConst

        entt::registry const& get_ent_registry() const { return ent_registry; }
        entt::registry& get_ent_registry() { return ent_registry; } // cppcheck-suppress functionConst

        virtual ~Application();

    protected:
        Application();
        virtual LayerVector get_layers() = 0;

    protected:
        Window window;

    private:
        void init();
        void handle(Event const&) override;

        LayerVector with_default_layers(LayerVector&&) const;

        void on_camera_constructed(entt::registry&, entt::entity);

    private:
        EventBus event_bus {};
        LayerStack layer_stack;

        entt::registry ent_registry {};

        bool running = false;
    };

    // defined by client
    scope<Application> CreateApplication();
}
