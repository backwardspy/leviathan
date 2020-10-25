#pragma once

#include "events.h"

namespace lv {
    class Layer {
    public:
        virtual ~Layer() noexcept = default;

        virtual void init() noexcept {}
        virtual void update() noexcept {}
        virtual bool handle(const Event&) noexcept { return false; }
    };
}
