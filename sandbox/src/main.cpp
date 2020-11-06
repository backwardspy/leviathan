#include <leviathan/leviathan.h>

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
        shader(app.get_render_context().make_shader("assets/shaders/unlit_generic.glsl")),
        material(app.get_render_context().make_material(shader)),
        quad_entity(app.get_ecs().make_entity()),
        tri_entity(app.get_ecs().make_entity()),
        cam(lv::Camera::make_orthographic(1.0f, app.get_window().get_aspect_ratio())) {
        auto& ctx = app.get_render_context();

        shader->set_alpha_blend(true);

        material->set_texture("MainTex", 0, ctx.make_texture("assets/textures/duck.png"));
        material->set_parameter("Tint", tint);

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
            auto scale = 16.0f;
            std::vector<lv::Vertex> verts {
                { glm::vec3(-0.5f, -0.5f, 0), glm::vec4(1), glm::vec2(0, scale) },
                { glm::vec3(0.5f, -0.5f, 0), glm::vec4(1), glm::vec2(scale) },
                { glm::vec3(-0.5f, 0.5f, 0), glm::vec4(1), glm::vec2(0) },
                { glm::vec3(0.5f, 0.5f, 0), glm::vec4(1), glm::vec2(scale, 0) },
            };

            std::vector<lv::Index> indices {
                0, 1, 2,
                1, 3, 2,
            };

            return context.make_vertex_array(std::move(verts), std::move(indices));
        }();

        auto& ecs = app.get_ecs();
        ecs.add_component(quad_entity, lv::ecs::Transform { glm::vec3(0), glm::identity<glm::quat>(), glm::vec3(5) });
        ecs.add_component(quad_entity, lv::ecs::Mesh { material, vertex_array });
        ecs.add_component(quad_entity, RotateOverTime {});
    }

    void init_tri_entity() {
        auto& ctx = app.get_render_context();
        auto vertex_array = [&ctx] {
            std::vector<lv::Vertex> verts {
                { glm::vec3(0, -0.4f, 0), glm::vec4(0.7f, 1, 0, 1), glm::vec2(0.5f, 1) },
                { glm::vec3(0.5f, 0.4f, 0), glm::vec4(0, 0.7f, 1, 1), glm::vec2(1, 0) },
                { glm::vec3(-0.5f, 0.4f, 0), glm::vec4(1, 0, 0.7f, 1), glm::vec2(0) },
            };

            return ctx.make_vertex_array(std::move(verts));
        }();

        auto mat = ctx.make_material(shader);
        mat->set_parameter("Tint", glm::vec4(1.0f));
        mat->set_texture("MainTex", 0, ctx.make_texture("assets/textures/pigeon.png"));

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

        ImGui::Begin("Shader");
        if (ImGui::ColorPicker4("Tint", glm::value_ptr(tint))) {
            material->set_parameter("Tint", tint);
        }
        ImGui::End();
    }

private:
    lv::Application& app;

    lv::ref<lv::Shader> shader;
    lv::ref<lv::Material> material;

    // shader params
    glm::vec4 tint = glm::vec4(1.0f);

    lv::ecs::Entity quad_entity, tri_entity;

    lv::Camera cam;
    float zoom = 0.0f;

    lv::ref<DragControlSystem> drag_control_system;
    lv::ref<RotateOverTimeSystem> rotate_over_time_system;
};

class Sandbox : public lv::Application {
protected:
    virtual lv::LayerVector get_layers() override {
        lv::LayerVector layers;
        layers.push_back(lv::make_scope<CustomLayer>(*this));
        return layers;
    }
};

lv::scope<lv::Application> lv::CreateApplication() {
    return lv::make_scope<Sandbox>();
}