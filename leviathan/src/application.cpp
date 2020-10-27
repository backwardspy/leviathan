#include "leviathan/lvpch.h"

#include "leviathan/application.h"
#include "leviathan/log.h"

#include "leviathan/layers/imgui_layer.h"

namespace lv {
    Application::Application(LayerVector&& layers) noexcept :
        event_bus {},
        running { false },
        window { WindowSettings{1280, 800, "Leviathan Application"}, event_bus },
        input { event_bus },
        layer_stack { event_bus, with_default_layers(std::move(layers)) }
    {
    }

    Application::~Application() noexcept {}

    int Application::run() noexcept {
        if (!init()) {
            Log::core_info("Quitting early due to unrecoverable issues during engine initialisation");
            return -1;
        }

        while (running) {
            event_bus.drain();

            window.update();

            layer_stack.pre_update();
            layer_stack.update();
            layer_stack.post_update();

            window.clear();

            layer_stack.pre_render();
            layer_stack.render();
            layer_stack.gui();
            layer_stack.post_render();

            window.display();

            input.end_frame();
        }

        return 0;
    }

    void Application::stop() noexcept {
        running = false;
    }

    bool Application::init() noexcept {
        Log::init();
        Log::core_info("Leviathan starting up");

        if (!window.create()) return false;

        event_bus.add_listener(*this);

        layer_stack.init();

        running = true;

        return true;
    }

    void Application::handle(const Event& event) noexcept {
        switch (event.type) {
            case Event::Type::WindowClosed:
                Log::core_info("Window was closed.");
                stop();
                break;
        }
    }

    LayerVector Application::with_default_layers(LayerVector&& layers) const {
        layers.emplace(std::begin(layers), std::make_unique<ImGuiLayer>(window));
        return layers;
    }
}