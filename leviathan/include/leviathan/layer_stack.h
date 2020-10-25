#pragma once

#include "lvpch.h"
#include "layer.h"
#include "window.h"

namespace lv {
    class LayerStack : public IEventListener {
    public:
        LayerStack(EventBus& event_bus) noexcept;

        void set_layers(std::vector<std::unique_ptr<Layer>> layers) noexcept;

        void init() noexcept;
        void update() noexcept;
        virtual void handle(const Event&) noexcept override;

    private:
        std::vector<std::unique_ptr<Layer>> layers;
    };
}
