#include "leviathan/lvpch.h"
#include "leviathan/window.h"

#if defined(LV_GRAPHICS_OPENGL)
#include "leviathan/platform/opengl/window.h"
using SelectedWindowImpl = lv::opengl::WindowImpl;
#else
#error Window requires an LV_GRAPHICS_* option defined in order to select a window implementation.
#endif

namespace lv {
    Window::Window(WindowSettings&& settings, EventBus& bus) :
        impl { make_scope<SelectedWindowImpl>(std::move(settings), bus) } {}
}