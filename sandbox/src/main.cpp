#include <leviathan/leviathan.h>
#include <imgui.h>

const std::string VertexShaderSource {
    R"glsl(
        #version 460 core

        layout(location = 0) in vec3 in_position;
        layout(location = 1) in vec4 in_color;

        out struct {
            vec3 position;
            vec4 color;
        } v2f;

        void main() {
            gl_Position = vec4(in_position, 1.0);
            v2f.position = in_position;
            v2f.color = in_color;
        }
    )glsl"
};

const std::string PixelShaderSource {
    R"glsl(
        #version 460 core

        out vec4 out_colour;

        in struct {
            vec3 position;
            vec4 color;
        } v2f;

        void main() {
            out_colour = v2f.color;
        }
    )glsl"
};

class CustomLayer : public lv::Layer {
public:
    explicit CustomLayer(lv::Context& context) :
        context { context },
        shader { context.create_shader(
            {{lv::Shader::Type::Vertex, VertexShaderSource },
            {lv::Shader::Type::Pixel, PixelShaderSource },}) },
        tri { context.create_vertex_array({
            { { 0.0f, 0.5f, 0.0f }, { 0.7f, 1.0f, 0.0f, 1.0f } },
            { { 0.5f, -0.4f, 0.0f }, { 0.0f, 0.7f, 1.0f, 1.0f } },
            { { -0.5f, -0.4f, 0.0f }, { 1.0f, 0.0f, 0.7f, 1.0f } },
        }) },
        quad { context.create_vertex_array({
            { { -0.75f, -0.75f, 0.0f }, { 1.0f, 0.7f, 0.0f, 1.0f } },
            { { 0.75f, -0.75f, 0.0f }, { 1.0f, 0.7f, 0.0f, 1.0f } },
            { { -0.75f, 0.75f, 0.0f }, { 1.0f, 0.7f, 0.0f, 1.0f } },
            { { 0.75f, 0.75f, 0.0f }, { 1.0f, 0.7f, 0.0f, 1.0f } },
        }, {
            0, 1, 2,
            1, 3, 2,
        }) }
    {}

    virtual void render() noexcept override {
        lv::Renderer::submit(*shader, *quad);
        lv::Renderer::submit(*shader, *tri);
    }

    virtual void gui() noexcept override {
        ImGui::Begin("Sandbox");
        ImGui::Text("Hello from Sandbox's custom layer!");
        ImGui::End();
    }

private:
    lv::Context& context;
    std::unique_ptr<lv::Shader> shader;
    std::unique_ptr<lv::VertexArray> tri, quad;
};

class Sandbox : public lv::Application {
protected:
    virtual lv::LayerVector get_layers() const override {
        lv::LayerVector layers;
        layers.emplace_back(std::make_unique<CustomLayer>(get_window().get_context()));
        return layers;
    }
};

std::unique_ptr<lv::Application> lv::CreateApplication() {
    return std::make_unique<Sandbox>();
}