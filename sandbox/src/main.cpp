#include <leviathan/leviathan.h>
#include <imgui.h>

const std::string VertexShaderSource {
    R"glsl(
        #version 460 core

        uniform mat4 vp;

        layout(location = 0) in vec3 in_position;
        layout(location = 1) in vec4 in_color;

        out struct {
            vec4 color;
        } v2f;

        void main() {
            gl_Position = vp * vec4(in_position, 1.0);
            v2f.color = in_color;
        }
    )glsl"
};

const std::string PixelShaderSource {
    R"glsl(
        #version 460 core

        out vec4 out_colour;

        in struct {
            vec4 color;
        } v2f;

        void main() {
            out_colour = v2f.color;
        }
    )glsl"
};

class CustomLayer : public lv::Layer {
public:
    explicit CustomLayer(const lv::Window& window) :
        Layer { "Custom" },
        context { window.get_context() },
        window { window },
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
        }) },
        cam { lv::Camera::make_orthographic(1.0f, window.get_aspect_ratio()) }
    {
        set_shader_vp();
    }

private:
    void set_shader_vp() {
        shader->set_mat4("vp", cam.get_vp_matrix());
    }

    bool handle(const lv::Event& event) noexcept override {
        switch (event.type) {
            case lv::Event::Type::MouseScrolled:
                zoom += event.mouse.coord.y;
                cam.set_ortho_size(pow(1.1f, -zoom));
                set_shader_vp();
                return true;
            case lv::Event::Type::ButtonPressed:
            case lv::Event::Type::ButtonReleased:
                if (event.button.code == lv::ButtonCode::Left)
                    dragging = event.type == lv::Event::Type::ButtonPressed;
                break;
            case lv::Event::Type::WindowResized:
                cam.set_aspect_ratio(window.get_aspect_ratio());
                set_shader_vp();
                break;
        }
        return false;
    }

    void update() noexcept override {
        if (dragging) {
            auto mouse_pos = lv::Input::get_mouse_position() / static_cast<glm::vec2>(window.get_size());
            auto cam_pos = -1.0f + 2.0f * mouse_pos;
            cam.set_position(glm::vec3(-cam_pos.x, cam_pos.y, 0.0f));
            set_shader_vp();
        }
    }

    void render() noexcept override {
        lv::Renderer::submit(*shader, *quad);
        lv::Renderer::submit(*shader, *tri);
    }

    void gui() noexcept override {
        ImGui::Begin("Sandbox");
        auto mouse_pos = lv::Input::get_mouse_position();
        auto mouse_uv = mouse_pos / static_cast<glm::vec2>(window.get_size());
        auto cam_pos = cam.get_position();
        ImGui::Text("Mouse: %.f, %.f (%.2f, %.2f)", mouse_pos.x, mouse_pos.y, mouse_uv.x, mouse_uv.y);
        ImGui::Text("Camera position: %.2f, %.2f", cam_pos.x, cam_pos.y);
        ImGui::Text("Zoom: %.f", zoom);
        ImGui::End();
    }

private:
    lv::Context& context;
    std::unique_ptr<lv::Shader> shader;
    std::unique_ptr<lv::VertexArray> tri, quad;
    lv::Camera cam;
    const lv::Window& window;
    bool dragging = false;
    float zoom = 0.0f;
};

class Sandbox : public lv::Application {
protected:
    virtual lv::LayerVector get_layers() const override {
        lv::LayerVector layers;
        layers.emplace_back(std::make_unique<CustomLayer>(get_window()));
        return layers;
    }
};

std::unique_ptr<lv::Application> lv::CreateApplication() {
    return std::make_unique<Sandbox>();
}