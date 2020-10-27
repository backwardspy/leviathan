#include "leviathan/lvpch.h"

#include <imgui.h>
#include <backends/imgui_impl_opengl3.h>
#include <backends/imgui_impl_glfw.h>

#include "leviathan/layers/imgui_layer.h"
#include "leviathan/log.h"

namespace lv {
    ImGuiLayer::ImGuiLayer(const Window& window) noexcept : window { window } {}

    void ImGuiLayer::init() noexcept {
        ImGui::CreateContext();
        ImGui::StyleColorsLight();
        ImGui_ImplGlfw_InitForOpenGL(window.get_glfw_handle(), true);
        ImGui_ImplOpenGL3_Init("#version 460");
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