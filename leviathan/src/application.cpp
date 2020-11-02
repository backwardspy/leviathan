#include "leviathan/lvpch.h"

#include "leviathan/application.h"
#include "leviathan/renderer.h"
#include "leviathan/log.h"
#include "leviathan/core/time.h"

#include "leviathan/layers/imgui_layer.h"

namespace lv {
    Application::Application() :
        event_bus {},
        window { { { 1280, 800 }, "Leviathan Application"}, event_bus },
        layer_stack { event_bus }
    {
    }

    Application::~Application() noexcept {}

    int Application::run() {
        init();

        auto accumulator = time::seconds { 0 };
        time::start();

        while (running) {
            event_bus.drain();
            window.update();

#pragma region Update
            layer_stack.pre_update();

            // TODO: world state will need some lerp applied to it to look smooth
            time::reset();
            accumulator += time::render_delta_time();
            int steps = 0;
            while (accumulator > time::delta_time) {
                layer_stack.update();
                accumulator -= time::delta_time;
                if (++steps >= time::max_fixed_steps) {
                    Log::warn("Simulation is running too slowly, some updates will be skipped!");
                    break;
                }
            }

            layer_stack.post_update();
#pragma endregion

#pragma region Rendering
            Renderer::clear();

            layer_stack.pre_render();
            layer_stack.render();
            layer_stack.gui();
            layer_stack.post_render();

            window.present();
#pragma endregion

            Input::end_frame();
        }

        return 0;
    }

    void Application::stop() noexcept {
        running = false;
    }

    void Application::init() {
        event_bus.add_listener(*this);
        Input::init(event_bus, window);
        layer_stack.init(with_default_layers(get_layers()));
        running = true;
        Log::core_info("Application starting up.");
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