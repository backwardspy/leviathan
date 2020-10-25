#pragma once

#include "leviathan/layer.h"
#include "leviathan/window.h"
#include "leviathan/input.h"

namespace lv {
    class ImGuiLayer : public Layer {
    public:
        ImGuiLayer(const Window&, const Input& input) noexcept;

        virtual void update() noexcept override;
        virtual bool handle(const Event&) noexcept override;

    private:
        const Input& input;
        double last_frame_time = 0.0;
    };
}
