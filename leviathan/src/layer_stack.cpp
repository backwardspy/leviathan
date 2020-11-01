#include "leviathan/lvpch.h"

#include "leviathan/layer_stack.h"
#include "leviathan/log.h"

namespace lv {
    LayerStack::LayerStack(EventBus& event_bus) noexcept {
        event_bus.add_listener(*this);
    }

    void LayerStack::init(LayerVector&& app_layers) noexcept {
        layers = std::move(app_layers);
        for (auto& layer : layers) {
            layer->init();
        }
        Log::core_info("Layer stack initialised with {} layer(s).", layers.size());
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
        for (auto& layer : layers) {
            if (layer->handle(event)) break;
        }
    }
}