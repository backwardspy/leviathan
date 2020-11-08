#pragma once

#include "lvpch.h"
#include "layer.h"
#include "core/time.h"

namespace lv {
    using LayerVector = std::vector<scope<Layer>>;

    class LayerStack : public IEventListener {
    public:
        explicit LayerStack(EventBus& event_bus);
        ~LayerStack();

        void init(LayerVector&&);

        void pre_update();
        void update();
        void post_update();

        void pre_render();
        void render();
        void gui();
        void post_render();

        void handle(Event const&) override;

    private:
        EventBus& event_bus;
        std::vector<scope<Layer>> layers;
    };
}
