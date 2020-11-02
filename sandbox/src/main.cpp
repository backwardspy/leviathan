#include <leviathan/leviathan.h>

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

struct Transform {
    glm::vec3 position;
};

struct DragControl {};

class DragControlSystem : public lv::ecs::System {
public:
    DragControlSystem(const lv::ecs::ECS& ecs, const lv::Window& window) : ecs { ecs }, window { window } {}

    void set_dragging(bool is_dragging) {
        dragging = is_dragging;
    }

    void update() const noexcept {
        for (auto const& entity : entities) {
            auto& transform = ecs.get_component<Transform>(entity);

            if (dragging) {
                auto mouse_pos = lv::Input::get_mouse_position() / static_cast<glm::vec2>(window.get_size());
                auto cam_pos = -1.0f + 2.0f * mouse_pos;
                transform.position = glm::vec3(-cam_pos.x, cam_pos.y, 0.0f);
            }
        }
    }

private:
    const lv::ecs::ECS& ecs;
    const lv::Window& window;
    bool dragging = false;
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

        ecs.register_component<DragControl>();
        ecs.register_component<Transform>();

        {
            lv::ecs::Archetype archetype;
            archetype.set(ecs.get_component_type<DragControl>());
            archetype.set(ecs.get_component_type<Transform>());
            drag_control_system = ecs.register_system<DragControlSystem>(archetype, ecs, window);
        }

        camera_entity = ecs.make_entity();
        ecs.add_component(camera_entity, Transform { {0, 0, 0} });
        ecs.add_component(camera_entity, DragControl {});
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
                    drag_control_system->set_dragging(event.type == lv::Event::Type::ButtonPressed);
                break;
            case lv::Event::Type::WindowResized:
                cam.set_aspect_ratio(window.get_aspect_ratio());
                set_shader_vp();
                break;
        }
        return false;
    }

    void render() noexcept override {
        drag_control_system->update();

        auto const& transform = ecs.get_component<Transform>(camera_entity);
        cam.set_position(transform.position);
        set_shader_vp();

        lv::Renderer::submit(*shader, *quad);
        lv::Renderer::submit(*shader, *tri);
    }

    void gui() noexcept override {
        auto mouse_pos = lv::Input::get_mouse_position();
        auto mouse_uv = mouse_pos / static_cast<glm::vec2>(window.get_size());
        auto cam_pos = cam.get_position();
        auto dt = lv::time::render_delta_time();
        ImGui::Begin("Sandbox");
        ImGui::Text("Mouse: %.f, %.f (%.3f, %.3f)", mouse_pos.x, mouse_pos.y, mouse_uv.x, mouse_uv.y);
        ImGui::Text("Camera position: %.3f, %.3f", cam_pos.x, cam_pos.y);
        ImGui::Text("Zoom: %.f", zoom);
        ImGui::Text("Performance: %.3f ms/frame (%.f fps)", lv::time::to_milliseconds(dt).count(), 1.0f / lv::time::to_seconds(dt).count());
        ImGui::Text("Simulation Time: %.3f", lv::time::to_seconds(lv::time::elapsed()).count());
        ImGui::End();
    }

private:
    lv::ecs::ECS ecs {};
    lv::Context& context;

    const lv::Window& window;

    std::unique_ptr<lv::Shader> shader;
    std::unique_ptr<lv::VertexArray> tri, quad;

    lv::Camera cam;
    lv::ecs::Entity camera_entity;
    float zoom { 0.0f };

    std::shared_ptr<DragControlSystem> drag_control_system;
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