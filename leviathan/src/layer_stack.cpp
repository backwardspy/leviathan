#include "leviathan/lvpch.h"

#include "leviathan/layer_stack.h"
#include "leviathan/log.h"

namespace lv {
    LayerStack::LayerStack(EventBus& event_bus) noexcept {
        event_bus.add_listener(*this);
    }

    void LayerStack::set_layers(std::vector<std::unique_ptr<Layer>> layers) noexcept {
        this->layers = std::move(layers);
        Log::core_info("Layer stack set up with {} layer(s)", this->layers.size());
    }

    void LayerStack::init() noexcept {
        for (auto& layer : layers) {
            layer->init();
        }
    }

    void LayerStack::update() noexcept {
        for (auto& layer : layers) {
            layer->update();
        }
    }

    void LayerStack::handle(const Event& event) noexcept {
        // reverse iterate so "higher" layers handle first
        for (auto layer = std::rbegin(layers); layer != std::rend(layers); layer++) {
            if ((*layer)->handle(event)) break;
        }
    }
}