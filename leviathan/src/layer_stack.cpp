#include "leviathan/lvpch.h"

#include "leviathan/layer_stack.h"
#include "leviathan/log.h"

namespace lv {
    LayerStack::LayerStack(EventBus& event_bus, LayerVector&& layers) noexcept : layers { std::move(layers) } {
        event_bus.add_listener(*this);
        Log::core_info("Layer stack set up with {} layer(s)", this->layers.size());
    }

    void LayerStack::init() noexcept {
        for (auto& layer : layers) {
            layer->init();
        }
    }

    void LayerStack::pre_update() noexcept {
        for (auto& layer : layers) {
            layer->pre_update();
        }
    }

    void LayerStack::update() noexcept {
        for (auto& layer : layers) {
            layer->update();
        }
    }

    void LayerStack::post_update() noexcept {
        for (auto& layer : layers) {
            layer->post_update();
        }
    }

    void LayerStack::pre_render() noexcept {
        for (auto& layer : layers) {
            layer->pre_render();
        }
    }

    void LayerStack::render() noexcept {
        for (auto& layer : layers) {
            layer->render();
        }
    }

    void LayerStack::gui() noexcept {
        for (auto& layer : layers) {
            layer->gui();
        }
    }

    void LayerStack::post_render() noexcept {
        for (auto& layer : layers) {
            layer->post_render();
        }
    }

    void LayerStack::handle(const Event& event) noexcept {
        // reverse iterate so "higher" layers handle first
        for (auto layer = std::rbegin(layers); layer != std::rend(layers); layer++) {
            if ((*layer)->handle(event)) break;
        }
    }
}