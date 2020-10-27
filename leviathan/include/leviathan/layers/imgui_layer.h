#pragma once

#include "leviathan/layer.h"
#include "leviathan/window.h"
#include "leviathan/input.h"

namespace lv {
    class ImGuiLayer : public Layer {
    public:
        ImGuiLayer(const Window&) noexcept;

        virtual void init() noexcept override;
        virtual void pre_render() noexcept override;
        virtual void post_render() noexcept override;

    private:
        const Window& window;
    };
}
