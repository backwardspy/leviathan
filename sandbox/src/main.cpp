#include <leviathan/leviathan.h>

const std::string VertexShaderSource {
    R"glsl(
        #version 460 core

        uniform mat4 Model;
        uniform mat4 ViewProjection;

        layout(location = 0) in vec3 in_position;
        layout(location = 1) in vec4 in_color;

        out struct {
            vec4 color;
        } v2f;

        void main() {
            gl_Position = ViewProjection * Model * vec4(in_position, 1.0);
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

struct DragControl {};

class DragControlSystem : public lv::ecs::System {
public:
    DragControlSystem(lv::ecs::ECS& ecs, const lv::Window& window) : ecs { ecs }, window { window } {}

    void set_dragging(bool is_dragging) {
        dragging = is_dragging;
    }

    void pre_render() const noexcept {
        for (auto const& entity : entities) {
            auto& transform = ecs.get_component<lv::ecs::Transform>(entity);

            if (dragging) {
                auto mouse_uv = lv::Input::get_mouse_position() / static_cast<glm::vec2>(window.get_size());
                mouse_uv.y = 1.0f - mouse_uv.y; // mouse Y and world Y are opposite
                transform.translation = glm::vec3(-1.0f + 2.0f * mouse_uv, 0.0f);
            }
        }
    }

    static lv::ecs::Archetype get_archetype(lv::ecs::ECS& ecs) {
        lv::ecs::Archetype archetype;
        archetype.set(ecs.get_component_type<DragControl>());
        archetype.set(ecs.get_component_type<lv::ecs::Transform>());
        return archetype;
    }

private:
    lv::ecs::ECS& ecs;
    const lv::Window& window;
    bool dragging = false;
};

struct RotateOverTime {
    float rate {};
};

class RotateOverTimeSystem : public lv::ecs::System {
public:
    RotateOverTimeSystem(lv::ecs::ECS& ecs) : ecs { ecs } {}

    void update() {
        auto dt = lv::time::to_seconds(lv::time::delta_time).count();
        for (const auto& entity : entities) {
            const auto& rot = ecs.get_component<RotateOverTime>(entity);
            auto& transform = ecs.get_component<lv::ecs::Transform>(entity);
            transform.rotation = glm::rotate(transform.rotation, rot.rate * dt, { 0, 0, 1 });
        }
    }

    static lv::ecs::Archetype get_archetype(lv::ecs::ECS& ecs) {
        lv::ecs::Archetype archetype;
        archetype.set(ecs.get_component_type<RotateOverTime>());
        archetype.set(ecs.get_component_type<lv::ecs::Transform>());
        return archetype;
    }

private:
    lv::ecs::ECS& ecs;
};

class CustomLayer : public lv::Layer {
public:
    explicit CustomLayer(lv::Application& app) :
        Layer { "Custom" },
        app { app },
        shader { app.get_render_context().create_shader(
            {
                {lv::Shader::Type::Vertex, VertexShaderSource },
                {lv::Shader::Type::Pixel, PixelShaderSource },
            }
        ) },
        quad_entity { app.get_ecs().make_entity() },
        tri_entity { app.get_ecs().make_entity() },
        cam { lv::Camera::make_orthographic(1.0f, app.get_window().get_aspect_ratio()) }
    {
        set_shader_vp();

        auto& window = app.get_window();
        auto& context = app.get_render_context();
        auto& ecs = app.get_ecs();

        ecs.register_component<DragControl>();
        ecs.register_component<RotateOverTime>();

        drag_control_system = ecs.register_system<DragControlSystem>(DragControlSystem::get_archetype(ecs), ecs, window);
        rotate_over_time_system = ecs.register_system<RotateOverTimeSystem>(RotateOverTimeSystem::get_archetype(ecs), ecs);

        auto quad = context.create_vertex_array(
            {
            { { -0.5f, -0.5f, 0.0f }, { 1.0f, 0.7f, 0.0f, 1.0f } },
            { { 0.5f, -0.5f, 0.0f }, { 1.0f, 0.7f, 0.0f, 1.0f } },
            { { -0.5f, 0.5f, 0.0f }, { 1.0f, 0.7f, 0.0f, 1.0f } },
            { { 0.5f, 0.5f, 0.0f }, { 1.0f, 0.7f, 0.0f, 1.0f } },
            }, {
                0, 1, 2,
                1, 3, 2,
            }
            );

        auto tri = context.create_vertex_array(
            {
            { { 0.0f, 0.3f, 0.0f }, { 0.7f, 1.0f, 0.0f, 1.0f } },
            { { 0.5f, -0.5f, 0.0f }, { 0.0f, 0.7f, 1.0f, 1.0f } },
            { { -0.5f, -0.5f, 0.0f }, { 1.0f, 0.0f, 0.7f, 1.0f } },
            }
        );

        ecs.add_component(quad_entity, lv::ecs::Transform {});
        ecs.add_component(quad_entity, lv::ecs::Mesh { shader, quad });
        ecs.add_component(quad_entity, RotateOverTime { 1.0f });

        ecs.add_component(tri_entity, lv::ecs::Transform {});
        ecs.add_component(tri_entity, lv::ecs::Mesh { shader, tri });
        ecs.add_component(tri_entity, DragControl {});
    }

private:
    void set_shader_vp() {
        // TODO: move into renderer/scene?
        shader->set_mat4("ViewProjection", cam.get_vp_matrix());
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
                cam.set_aspect_ratio(app.get_window().get_aspect_ratio());
                set_shader_vp();
                break;
        }
        return false;
    }

    void update() noexcept override {
        rotate_over_time_system->update();
    }

    void pre_render() noexcept override {
        drag_control_system->pre_render();
    }

    void gui() noexcept override {
        auto mouse_pos = lv::Input::get_mouse_position();
        auto mouse_uv = mouse_pos / static_cast<glm::vec2>(app.get_window().get_size());
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
    lv::Application& app;

    std::shared_ptr<lv::Shader> shader;

    lv::ecs::Entity quad_entity, tri_entity;

    lv::Camera cam;
    float zoom = 0.0f;

    std::shared_ptr<DragControlSystem> drag_control_system;
    std::shared_ptr<RotateOverTimeSystem> rotate_over_time_system;
};

class Sandbox : public lv::Application {
protected:
    virtual lv::LayerVector get_layers() override {
        lv::LayerVector layers;
        layers.emplace_back(std::make_unique<CustomLayer>(*this));
        return layers;
    }
};

std::unique_ptr<lv::Application> lv::CreateApplication() {
    return std::make_unique<Sandbox>();
}