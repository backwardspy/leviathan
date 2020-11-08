#include "leviathan/lvpch.h"
#include "leviathan/layer_stack.h"

namespace lv {
    LayerStack::LayerStack(EventBus& event_bus) : event_bus(event_bus) {
        event_bus.add_listener(this);
    }

    LayerStack::~LayerStack() {
        event_bus.remove_listener(this);
    }

    void LayerStack::init(LayerVector&& app_layers) {
        layers = std::move(app_layers);
        for (auto& layer : layers) {
            layer->init();
        }
        Log::core_info("Layer stack initialised with {} layer(s).", layers.size());
    }

    void LayerStack::pre_update() {
        for (auto& layer : layers) {
            layer->pre_update();
        }
    }

    void LayerStack::update() {
        for (auto& layer : layers) {
            layer->update();
        }
    }

    void LayerStack::post_update() {
        for (auto& layer : layers) {
            layer->post_update();
        }
    }

    void LayerStack::pre_render() {
        for (auto& layer : layers) {
            layer->pre_render();
        }
    }

    void LayerStack::render() {
        for (auto& layer : layers) {
            layer->render();
        }
    }

    void LayerStack::gui() {
        for (auto& layer : layers) {
            layer->gui();
        }
    }

    void LayerStack::post_render() {
        for (auto& layer : layers) {
            layer->post_render();
        }
    }

    void LayerStack::handle(Event const& event) {
        for (auto& layer : layers) {
            if (layer->handle(event)) break;
        }
    }
}