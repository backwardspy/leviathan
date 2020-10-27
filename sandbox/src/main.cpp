#include <leviathan/leviathan.h>
#include <imgui.h>

class CustomLayer : public lv::Layer {
public:
    virtual void gui() noexcept override {
        ImGui::Begin("Sandbox");
        ImGui::Text("Hello from Sandbox's custom layer!");
        ImGui::End();
    }
};

class Sandbox : public lv::Application {
public:
    Sandbox() : lv::Application { get_layers() } {}

private:
    lv::LayerVector get_layers() const {
        lv::LayerVector layers;
        layers.emplace_back(std::make_unique<CustomLayer>());
        return layers;
    }
};

std::unique_ptr<lv::Application> lv::CreateApplication() noexcept {
    return std::make_unique<Sandbox>();
}