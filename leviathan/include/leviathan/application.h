#pragma once

#include "events.h"
#include "input.h"
#include "layer_stack.h"
#include "window.h"
#include "ecs/ecs.h"
#include "ecs/default_systems.h"
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

        constexpr ecs::ECS const& get_ecs() const { return ecs; }
        constexpr ecs::ECS& get_ecs() { return ecs; }   // cppcheck-suppress functionConst

        virtual ~Application() = default;

    protected:
        Application();
        virtual LayerVector get_layers() = 0;

    protected:
        Window window;
        ecs::ECS ecs {};

    private:
        void init();
        void handle(Event const&) override;

        LayerVector with_default_layers(LayerVector&&) const;

    private:
        EventBus event_bus {};
        LayerStack layer_stack;

        // systems
        std::shared_ptr<ecs::MeshRenderer> mesh_renderer;

        bool running = false;
    };

    // defined by client
    std::unique_ptr<Application> CreateApplication();
}
