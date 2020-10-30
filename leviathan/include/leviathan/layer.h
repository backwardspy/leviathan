#pragma once

#include "events.h"

namespace lv {
    class Layer {
    public:
        virtual ~Layer() noexcept = default;

        virtual void init() {}

        virtual void pre_update() noexcept {}
        virtual void update() noexcept {}
        virtual void post_update() noexcept {}

        virtual void pre_render() noexcept {}
        virtual void gui() noexcept {}
        virtual void render() noexcept {}
        virtual void post_render() noexcept {}

        virtual bool handle(const Event&) noexcept { return false; }
    };
}
