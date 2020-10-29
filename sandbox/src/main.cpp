#include <leviathan/leviathan.h>
#include <imgui.h>

const std::string VertexShaderSource {
    R"glsl(
        #version 460 core

        in vec3 in_position;

        out struct {
            vec2 position;
        } v2f;

        void main() {
            gl_Position = vec4(in_position, 1.0);
            v2f.position = in_position.xy;
        }
    )glsl"
};

const std::string PixelShaderSource {
    R"glsl(
        #version 460 core

        out vec4 out_colour;

        in struct {
            vec2 position;
        } v2f;

        void main() {
            vec3 col1 = vec3(255.0 / 255.0, 113.0 / 255.0, 206.0 / 255.0);
            vec3 col2 = vec3(1.0 / 255.0, 205.0 / 255.0, 254.0 / 255.0);
            out_colour = vec4(mix(col1, col2, -v2f.position.y * 0.2 + v2f.position.x * 2.0), 1.0);
        }
    )glsl"
};

class CustomLayer : public lv::Layer {
public:
    CustomLayer(lv::RenderContext& context) :
        context { context },
        shader { context.create_shader(
            {{lv::Shader::Type::Vertex, VertexShaderSource },
            {lv::Shader::Type::Pixel, PixelShaderSource },}) },
        vbo { context.create_vertex_buffer({
            { { 0.0f, 0.5f, 0.0f } },
            { { 0.5f, -0.4f, 0.0f } },
            { { -0.5f, -0.4f, 0.0f } },
        }) },
        ibo { context.create_index_buffer({ 0, 1, 2 }) }
    {}

    virtual void render() noexcept override {
        context.draw_indexed(lv::RenderMode::Triangles, *vbo, *ibo, *shader);
    }

    virtual void gui() noexcept override {
        ImGui::Begin("Sandbox");
        ImGui::Text("Hello from Sandbox's custom layer!");
        ImGui::End();
    }

private:
    lv::RenderContext& context;
    std::unique_ptr<lv::Shader> shader;
    std::unique_ptr<lv::VertexBuffer> vbo;
    std::unique_ptr<lv::IndexBuffer> ibo;
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