#pragma once

#include "leviathan/layer.h"
#include "leviathan/input.h"
#include "leviathan/window.h"

namespace lv {
    class ImGuiLayer : public Layer {
    public:
        explicit ImGuiLayer(const Window&) noexcept;

        void init() override;
        void pre_render() noexcept override;
        void post_render() noexcept override;

    private:
        const Window& window;
    };
}
