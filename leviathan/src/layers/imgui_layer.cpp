#include "leviathan/lvpch.h"

#include <imgui.h>

#if defined(LV_GRAPHICS_OPENGL)
#include <backends/imgui_impl_opengl3.h>
#include <backends/imgui_impl_glfw.h>
#include <GLFW/glfw3.h>
#else
#error ImGuiLayer requires an LV_GRAPHICS_* option defined in order to select an ImGui backend.
#endif

#include "leviathan/layers/imgui_layer.h"
#include "leviathan/log.h"

namespace lv {
    ImGuiLayer::ImGuiLayer(Window const& window) : Layer { "ImGui" }, window { window } {}

    void ImGuiLayer::init() {
        ImGui::CreateContext();
        ImGui::StyleColorsLight();

#if defined(LV_GRAPHICS_OPENGL)
        auto handle = std::any_cast<GLFWwindow*>(window.get_native_handle());
        ImGui_ImplGlfw_InitForOpenGL(handle, false);
        ImGui_ImplOpenGL3_Init("#version 460");
#endif
    }

    void ImGuiLayer::pre_render() {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
    }

    void ImGuiLayer::post_render() {
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }

    bool ImGuiLayer::handle(Event const& event) {
        auto const& io = ImGui::GetIO();
        return
            (event.is_in_category(EventCategory::Keyboard) && io.WantCaptureKeyboard) ||
            (event.is_in_category(EventCategory::Mouse) && io.WantCaptureMouse);
    }
}