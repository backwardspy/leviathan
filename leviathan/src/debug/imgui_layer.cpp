#include "leviathan/lvpch.h"

#include <imgui.h>
#include <backends/imgui_impl_opengl3.h>
#include <GLFW/glfw3.h>

#include "leviathan/debug/imgui_layer.h"
#include "leviathan/log.h"

namespace lv {
    ImGuiLayer::ImGuiLayer(const Window& window, const Input& input) noexcept : input { input } {
        auto context = ImGui::CreateContext();
        ImGui::StyleColorsLight();

        auto& io = ImGui::GetIO();
        io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
        io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;

        // TEMPORARY: replace with lv::KeyCode mapping
        io.KeyMap[ImGuiKey_Tab] = GLFW_KEY_TAB;
        io.KeyMap[ImGuiKey_LeftArrow] = GLFW_KEY_LEFT;
        io.KeyMap[ImGuiKey_RightArrow] = GLFW_KEY_RIGHT;
        io.KeyMap[ImGuiKey_UpArrow] = GLFW_KEY_UP;
        io.KeyMap[ImGuiKey_DownArrow] = GLFW_KEY_DOWN;
        io.KeyMap[ImGuiKey_PageUp] = GLFW_KEY_PAGE_UP;
        io.KeyMap[ImGuiKey_PageDown] = GLFW_KEY_PAGE_DOWN;
        io.KeyMap[ImGuiKey_Home] = GLFW_KEY_HOME;
        io.KeyMap[ImGuiKey_End] = GLFW_KEY_END;
        io.KeyMap[ImGuiKey_Insert] = GLFW_KEY_INSERT;
        io.KeyMap[ImGuiKey_Delete] = GLFW_KEY_DELETE;
        io.KeyMap[ImGuiKey_Backspace] = GLFW_KEY_BACKSPACE;
        io.KeyMap[ImGuiKey_Space] = GLFW_KEY_SPACE;
        io.KeyMap[ImGuiKey_Enter] = GLFW_KEY_ENTER;
        io.KeyMap[ImGuiKey_Escape] = GLFW_KEY_ESCAPE;
        io.KeyMap[ImGuiKey_KeyPadEnter] = GLFW_KEY_KP_ENTER;
        io.KeyMap[ImGuiKey_A] = GLFW_KEY_A;
        io.KeyMap[ImGuiKey_C] = GLFW_KEY_C;
        io.KeyMap[ImGuiKey_V] = GLFW_KEY_V;
        io.KeyMap[ImGuiKey_X] = GLFW_KEY_X;
        io.KeyMap[ImGuiKey_Y] = GLFW_KEY_Y;
        io.KeyMap[ImGuiKey_Z] = GLFW_KEY_Z;

        io.DisplaySize = { (float) window.get_width(), (float) window.get_height() };

        ImGui_ImplOpenGL3_Init("#version 460");
    }

    void ImGuiLayer::update() noexcept {
        auto time = glfwGetTime();

        auto& io = ImGui::GetIO();

        io.DeltaTime = last_frame_time > 0 ? float(time - last_frame_time) : 1.0f / 60.0f;
        last_frame_time = time;

        ImGui_ImplOpenGL3_NewFrame();
        ImGui::NewFrame();

        io.MouseDown[ImGuiMouseButton_Left] = input.is_button_pressed(ButtonCode::Left);
        io.MouseDown[ImGuiMouseButton_Right] = input.is_button_pressed(ButtonCode::Right);
        io.MousePos.x = (float) input.get_mouse_x();
        io.MousePos.y = (float) input.get_mouse_y();

        static bool show = true;
        ImGui::ShowDemoWindow(&show);

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }

    bool ImGuiLayer::handle(const Event& event) noexcept {
        auto& io = ImGui::GetIO();
        switch (event.type) {
            case Event::Type::WindowResized:
                io.DisplaySize = { (float) event.window.width, (float) event.window.height };
                break;
            case Event::Type::KeyPressed:
            case Event::Type::KeyReleased:
                io.KeysDown[(size_t) event.key.code] = event.type == Event::Type::KeyPressed;
                io.KeyShift = event.key.shift;
                io.KeyCtrl = event.key.control;
                io.KeyAlt = event.key.alt;
                io.KeySuper = event.key.super;
                break;
            case Event::Type::TextEntered:
                io.AddInputCharacter(event.text.codepoint);
                break;
            case Event::Type::MouseScrolled:
                io.MouseWheel = (float) event.mouse.y;
                io.MouseWheelH = (float) event.mouse.x;
                break;
        }
        return false;
    }
}