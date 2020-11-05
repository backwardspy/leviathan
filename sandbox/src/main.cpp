#include <leviathan/leviathan.h>

std::string const VertexShaderSource {
    R"glsl(
        #version 460 core

        uniform mat4 Model;
        uniform mat4 ViewProjection;
        uniform vec4 Tint;

        layout(location = 0) in vec3 in_position;
        layout(location = 1) in vec4 in_color;

        out struct {
            vec4 color;
        } v2f;

        void main() {
            gl_Position = ViewProjection * Model * vec4(in_position, 1.0);
            v2f.color = in_color * Tint;
        }
    )glsl"
};

std::string const PixelShaderSource {
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

lv::Shader::SourceMap const ShaderSourceMap {
    {lv::Shader::Type::Vertex, VertexShaderSource },
    {lv::Shader::Type::Pixel, PixelShaderSource },
};

struct DragControl {};

class DragControlSystem : public lv::ecs::System {
public:
    DragControlSystem(lv::ecs::ECS& ecs, lv::Window const& window) : ecs { ecs }, window { window } {}

    void set_dragging(bool is_dragging) {
        dragging = is_dragging;
    }

    void pre_render() const {
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
    lv::Window const& window;
    bool dragging = false;
};

struct RotateOverTime {
    float rate = 1.0f;
};

class RotateOverTimeSystem : public lv::ecs::System {
public:
    explicit RotateOverTimeSystem(lv::ecs::ECS& ecs) : ecs { ecs } {}

    void update() {
        auto dt = lv::time::to_seconds(lv::time::delta_time).count();
        for (auto const& entity : entities) {
            auto const& rot = ecs.get_component<RotateOverTime>(entity);
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
        Layer("Custom"),
        app(app),
        shader(app.get_render_context().make_shader(ShaderSourceMap)),
        material(app.get_render_context().make_material(shader)),
        quad_entity(app.get_ecs().make_entity()),
        tri_entity(app.get_ecs().make_entity()),
        cam(lv::Camera::make_orthographic(1.0f, app.get_window().get_aspect_ratio())) {
        material->set_parameter<glm::vec4>("Tint", quad_color);

        set_shader_vp();

        register_components();
        register_systems();

        init_entities();
    }

private:
    void set_shader_vp() {
        // TODO: move into renderer/scene?
        shader->set_mat4("ViewProjection", cam.get_vp_matrix());
    }

    void register_components() {
        auto& ecs = app.get_ecs();
        ecs.register_component<DragControl>();
        ecs.register_component<RotateOverTime>();
    }

    void register_systems() {
        auto& ecs = app.get_ecs();
        drag_control_system = ecs.register_system<DragControlSystem>(DragControlSystem::get_archetype(ecs), ecs, app.get_window());
        rotate_over_time_system = ecs.register_system<RotateOverTimeSystem>(RotateOverTimeSystem::get_archetype(ecs), ecs);
    }

    void init_entities() {
        init_quad_entity();
        init_tri_entity();
    }

    void init_quad_entity() {
        auto& context = app.get_render_context();
        auto vertex_array = [&context] {
            std::vector<lv::Vertex> verts {
                { glm::vec3(-0.5f, -0.5f, 0), glm::vec4(1) },
                { glm::vec3(0.5f, -0.5f, 0), glm::vec4(1) },
                { glm::vec3(-0.5f, 0.5f, 0), glm::vec4(1) },
                { glm::vec3(0.5f, 0.5f, 0), glm::vec4(1) },
            };

            std::vector<lv::Index> indices {
                0, 1, 2,
                1, 3, 2,
            };

            return context.make_vertex_array(std::move(verts), std::move(indices));
        }();

        auto& ecs = app.get_ecs();
        ecs.add_component(quad_entity, lv::ecs::Transform {});
        ecs.add_component(quad_entity, lv::ecs::Mesh { material, vertex_array });
        ecs.add_component(quad_entity, RotateOverTime {});
    }

    void init_tri_entity() {
        auto& context = app.get_render_context();
        auto vertex_array = [&context] {
            std::vector<lv::Vertex> verts {
                { glm::vec3(0, 0.3f, 0), glm::vec4(0.7f, 1, 0, 1) },
                { glm::vec3(0.5f, -0.5f, 0), glm::vec4(0, 0.7f, 1, 1) },
                { glm::vec3(-0.5f, -0.5f, 0), glm::vec4(1, 0, 0.7f, 1) },
            };

            return context.make_vertex_array(std::move(verts));
        }();

        auto mat = context.make_material(shader);
        mat->set_parameter("Tint", glm::vec4(1.0f));

        auto& ecs = app.get_ecs();
        ecs.add_component(tri_entity, lv::ecs::Transform {});
        ecs.add_component(tri_entity, lv::ecs::Mesh { mat, vertex_array });
        ecs.add_component(tri_entity, DragControl {});
    }

    bool handle(lv::Event const& event) override {
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

    void update() override {
        rotate_over_time_system->update();
    }

    void pre_render() override {
        drag_control_system->pre_render();
    }

    void gui() override {
        auto const mouse_pos = lv::Input::get_mouse_position();
        auto const cam_pos = cam.get_position();
        auto const dt = lv::time::render_delta_time();

        ImGui::Begin("Sandbox");
        ImGui::Text("Mouse: %.f, %.f", mouse_pos.x, mouse_pos.y);
        ImGui::Text("Camera position: %.3f, %.3f", cam_pos.x, cam_pos.y);
        ImGui::Text("Zoom: %.f", zoom);
        ImGui::Text("Performance: %.3f ms/frame (%.f fps)", lv::time::to_milliseconds(dt).count(), 1.0f / lv::time::to_seconds(dt).count());
        ImGui::Text("Simulation Time: %.3f", lv::time::to_seconds(lv::time::elapsed()).count());
        ImGui::End();

        ImGui::Begin("Quad");
        if (ImGui::ColorEdit4("Quad Colour", glm::value_ptr(quad_color))) {
            material->set_parameter("Tint", quad_color);
        }
        ImGui::End();
    }

private:
    lv::Application& app;

    std::shared_ptr<lv::Shader> shader;
    std::shared_ptr<lv::Material> material;

    lv::ecs::Entity quad_entity, tri_entity;
    glm::vec4 quad_color = glm::vec4(1);

    lv::Camera cam;
    float zoom = 0.0f;

    std::shared_ptr<DragControlSystem> drag_control_system;
    std::shared_ptr<RotateOverTimeSystem> rotate_over_time_system;
};

class Sandbox : public lv::Application {
protected:
    virtual lv::LayerVector get_layers() override {
        lv::LayerVector layers;
        layers.push_back(std::make_unique<CustomLayer>(*this));
        return layers;
    }
};

std::unique_ptr<lv::Application> lv::CreateApplication() {
    return std::make_unique<Sandbox>();
}