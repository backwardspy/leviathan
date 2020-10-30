#pragma once

#include "lvpch.h"
#include "layer.h"

namespace lv {
    using LayerVector = std::vector<std::unique_ptr<Layer>>;

    class LayerStack : public IEventListener {
    public:
        explicit LayerStack(EventBus& event_bus) noexcept;

        void init(LayerVector&&) noexcept;

        void pre_update() noexcept;
        void update() noexcept;
        void post_update() noexcept;

        void pre_render() noexcept;
        void render() noexcept;
        void gui() noexcept;
        void post_render() noexcept;

        void handle(const Event&) noexcept override;

    private:
        std::vector<std::unique_ptr<Layer>> layers;
    };
}
