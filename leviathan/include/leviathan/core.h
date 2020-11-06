#pragma once

#include <memory>

#if defined(_WIN32)
#define LV_PLATFORM_WINDOWS
#else
#define LV_PLATFORM_UNKNOWN
#endif

#if defined(LV_GRAPHICS_OPENGL)
constexpr int LVGLVersionMajor = 4;
constexpr int LVGLVersionMinor = 6;
#endif

namespace lv {
    template<class T> using ref = std::shared_ptr<T>;
    template<class T> using scope = std::unique_ptr<T>;

    template<class T, class... Args>
    static inline ref<T> make_ref(Args&&... args) { return std::make_shared<T>(std::forward<Args>(args)...); }

    template<class T, class... Args>
    static inline scope<T> make_scope(Args&&... args) { return std::make_unique<T>(std::forward<Args>(args)...); }
}

#include "leviathan/log.h"
#include "leviathan/exc.h"
