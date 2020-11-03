#pragma once

#include "events.h"
#include "input.h"
#include "layer_stack.h"
#include "window.h"
#include "ecs/ecs.h"
#include "ecs/default_systems.h"

namespace lv {
    class Application : public IEventListener {
    public:
        Application(const Application&&) noexcept = delete;
        void operator=(const Application&&) noexcept = delete;

        int run();
        void stop() noexcept;

        constexpr Window& get_window() { return window; }
        Context& get_render_context() { return window.get_context(); }
        constexpr ecs::ECS& get_ecs() { return ecs; }

        virtual ~Application() noexcept;

    protected:
        Application();
        virtual LayerVector get_layers() = 0;

    protected:
        Window window;
        ecs::ECS ecs {};

    private:
        void init();
        void handle(const Event&) noexcept override;

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
