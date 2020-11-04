#pragma once

#include "events.h"
#include "core/time.h"

namespace lv {
    class Layer {
    public:
        explicit Layer(std::string&& name) : name { std::move(name) } {}
        virtual ~Layer() = default;

        virtual void init() {}

        virtual void pre_update() {}
        virtual void update() {}
        virtual void post_update() {}

        virtual void pre_render() {}
        virtual void gui() {}
        virtual void render() {}
        virtual void post_render() {}

        virtual bool handle(Event const&) { return false; }

        std::string get_name() const { return name; }

    private:
        std::string name;
    };
}
