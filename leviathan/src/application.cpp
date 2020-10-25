#include "leviathan/lvpch.h"

#include "leviathan/application.h"
#include "leviathan/log.h"

#include "leviathan/debug/imgui_layer.h"

namespace lv {
    Application::Application() noexcept :
        event_bus {},
        running { false },
        window { WindowSettings{1280, 800, "Leviathan Application"}, event_bus },
        input { event_bus },
        layers { event_bus }
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
            window.clear();
            layers.update();
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

        std::vector<std::unique_ptr<Layer>> layer_stack;
        layer_stack.emplace_back(std::make_unique<ImGuiLayer>(window, input));
        layers.set_layers(std::move(layer_stack));

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
}