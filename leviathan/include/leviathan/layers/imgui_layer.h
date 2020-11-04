#pragma once

#include "leviathan/layer.h"
#include "leviathan/input.h"
#include "leviathan/window.h"
#include "leviathan/events.h"

namespace lv {
    class ImGuiLayer : public Layer {
    public:
        explicit ImGuiLayer(Window const&);

        void init() override;
        void pre_render() override;
        void post_render() override;
        bool handle(Event const&) override;

    private:
        Window const& window;
    };
}
