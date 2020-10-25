#pragma once

#include "events.h"
#include "window.h"
#include "input.h"
#include "layer_stack.h"

#include "debug/imgui_layer.h"

namespace lv {
    class LV_API Application : public IEventListener {
    public:
        Application(const Application&&) noexcept = delete;
        void operator=(const Application&&) noexcept = delete;

        int run() noexcept;
        void stop() noexcept;

        constexpr const Window& get_window() const noexcept { return window; }

        virtual ~Application() noexcept;

    protected:
        Application() noexcept;

    private:
        bool init() noexcept;

        virtual void handle(const Event&) noexcept override;

        EventBus event_bus;
        Window window;
        Input input;
        LayerStack layers;

        bool running;
    };

    // defined by client
    std::unique_ptr<Application> CreateApplication() noexcept;
}
