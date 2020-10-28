#pragma once

namespace lv {
    class RenderContext {
    public:
        virtual ~RenderContext() noexcept {}

        virtual bool init() noexcept = 0;
        virtual void make_current() noexcept = 0;
        virtual void present() noexcept = 0;
    };
}
