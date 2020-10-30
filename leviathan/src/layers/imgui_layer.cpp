#include "leviathan/lvpch.h"

#include <imgui.h>

#if defined(LV_GRAPHICS_OPENGL)
#include <backends/imgui_impl_opengl3.h>
#include <backends/imgui_impl_glfw.h>
#else
#error ImGuiLayer requires an LV_GRAPHICS_* option defined in order to select an ImGui backend.
#endif

#include "leviathan/layers/imgui_layer.h"
#include "leviathan/log.h"

namespace lv {
    ImGuiLayer::ImGuiLayer(const Window& window) noexcept : window { window } {}

    void ImGuiLayer::init() {
        ImGui::CreateContext();
        ImGui::StyleColorsLight();

#if defined(LV_GRAPHICS_OPENGL)
        ImGui_ImplGlfw_InitForOpenGL(std::any_cast<GLFWwindow*>(window.get_native_handle()), true);
        ImGui_ImplOpenGL3_Init("#version 460");
#endif
    }

    void ImGuiLayer::pre_render() noexcept {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
    }

    void ImGuiLayer::post_render() noexcept {
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }
}