#pragma once

#include "events.h"
#include "core/time.h"

namespace lv {
    class Layer {
    public:
        Layer(std::string&& name) : name { std::move(name) } {}
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

        std::string get_name() const { return name; }

    private:
        std::string name;
    };
}
