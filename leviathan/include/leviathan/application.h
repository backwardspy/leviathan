#pragma once

#include "events.h"
#include "window.h"
#include "input.h"
#include "layer_stack.h"

namespace lv {
    class Application : public IEventListener {
    public:
        Application(const Application&&) noexcept = delete;
        void operator=(const Application&&) noexcept = delete;

        int run() noexcept;
        void stop() noexcept;

        constexpr const Window& get_window() const noexcept { return window; }

        virtual ~Application() noexcept;

    protected:
        Application();
        virtual LayerVector get_layers() const = 0;

    private:
        void init() noexcept;

        virtual void handle(const Event&) noexcept override;

        LayerVector with_default_layers(LayerVector&&) const;

        EventBus event_bus;
        Window window;
        Input input;
        LayerStack layer_stack;

        bool running;
    };

    // defined by client
    std::unique_ptr<Application> CreateApplication();
}
