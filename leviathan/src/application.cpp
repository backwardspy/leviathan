#include "leviathan/lvpch.h"
#include "leviathan/application.h"
#include "leviathan/renderer/renderer.h"
#include "leviathan/core/time.h"
#include "leviathan/scene/components.h"

#include "leviathan/layers/imgui_layer.h"

namespace lv {
    Application::Application() :
        window { { { 1280, 800 }, "Leviathan Application"}, event_bus },
        layer_stack { event_bus }
    {
        event_bus.add_listener(this);

        ent_registry.on_construct<Camera>().connect<&Application::on_camera_constructed>(*this);
    }

    Application::~Application() {
        // event_bus is about to be deleted anyway, but it can't hurt to clean things up properly.
        event_bus.remove_listener(this);
    }

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
                    Log::core_warn("Simulation is running too slowly, some updates will be skipped!");
                    break;
                }
            }

            layer_stack.post_update();
#pragma endregion

#pragma region Rendering
            renderer::clear();

            layer_stack.pre_render();
            layer_stack.render();

            // TODO: this should probably be its own layer or something
            auto cameras = ent_registry.view<Transform, Camera>();
            auto meshes = ent_registry.view<Transform, MeshRenderer>();
            for (auto ent : cameras) {
                auto const& [camera_transform, camera] = cameras.get<Transform, Camera>(ent);
                if (!camera.active) continue;
                auto view_projection = camera.get_projection() * camera_transform.inverse_matrix();
                for (auto ent : meshes) {
                    auto const& [model, mesh] = meshes.get<Transform, MeshRenderer>(ent);
                    renderer::submit(
                        *mesh.material,
                        *mesh.vertex_array,
                        model.matrix(),
                        view_projection
                    );
                }
            }

            layer_stack.gui();
            layer_stack.post_render();

            window.present();
#pragma endregion

            Input::end_frame();
        }

        return 0;
    }

    void Application::stop() {
        running = false;
    }

    void Application::init() {
        Input::init(event_bus, window);
        layer_stack.init(with_default_layers(get_layers()));
        running = true;
        Log::core_info("Application starting up.");
    }

    void Application::handle(Event const& event) {
        switch (event.type) {
            case Event::Type::WindowClosed:
                Log::core_info("Window was closed.");
                stop();
                break;
        }
    }

    LayerVector Application::with_default_layers(LayerVector&& layers) const {
        layers.insert(std::begin(layers), make_scope<ImGuiLayer>(window));
        return layers;
    }

    void Application::on_camera_constructed(entt::registry& registry, entt::entity entity) {
        // if this is the only camera, set it active.
        if (registry.view<Camera>().size() == 1) {
            auto& cam = registry.get<Camera>(entity);
            cam.active = true;
        }
    }
}