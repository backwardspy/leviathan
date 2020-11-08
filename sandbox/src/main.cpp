#include <leviathan/leviathan.h>

struct CameraController2D {
    float speed = 1.0f;
    int zoom_level = 0;
};

class CameraController2DSystem : public lv::ecs::System {
public:
    explicit CameraController2DSystem(lv::ecs::ECS& ecs) : ecs { ecs } {}

    void update() {
        auto dt = lv::time::to_seconds(lv::time::delta_time).count();

        glm::vec2 motion;
        motion.x = static_cast<float>(lv::Input::key_axis(lv::KeyCode::D, lv::KeyCode::A));
        motion.y = static_cast<float>(lv::Input::key_axis(lv::KeyCode::W, lv::KeyCode::S));

        for (auto const& entity : entities) {
            auto const& controller = ecs.get_component<CameraController2D>(entity);
            auto zoom = pow(1.1f, -controller.zoom_level);

            auto& transform = ecs.get_component<lv::ecs::Transform>(entity);
            auto translation = glm::translate(glm::identity<glm::mat4>(), glm::vec3(motion * controller.speed * zoom * dt, 0));
            transform.translation = translation * glm::vec4(transform.translation, 1);

            auto& cam = ecs.get_component<lv::ecs::Camera>(entity);
            cam.camera->set_ortho_size(zoom);
            cam.camera->set_position(transform.translation);    // TODO: get rid of this duplication
        }
    }

    static lv::ecs::Archetype get_archetype(lv::ecs::ECS& ecs) {
        lv::ecs::Archetype archetype;
        archetype.set(ecs.get_component_type<CameraController2D>());
        archetype.set(ecs.get_component_type<lv::ecs::Transform>());
        archetype.set(ecs.get_component_type<lv::ecs::Camera>());
        return archetype;
    }

private:
    lv::ecs::ECS& ecs;
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
            transform.rotation = glm::rotate(transform.rotation, rot.rate * dt, glm::vec3(0, 0, 1));
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
        shader(app.get_render_context().make_shader("assets/shaders/unlit_generic.glsl", { lv::Shader::Type::Vertex, lv::Shader::Type::Pixel })),
        material(app.get_render_context().make_material(shader)),
        quad(app.get_ecs().make_entity()),
        tri(app.get_ecs().make_entity()),
        cam(app.get_ecs().make_entity()) {
        shader->set_alpha_blend(true);

        auto& ctx = app.get_render_context();
        material->set_texture("MainTex", 0, ctx.make_texture("assets/textures/duck.png"));
        material->set_parameter("Tint", tint);

        register_components();
        register_systems();
        init_entities();

        set_shader_vp();
    }

private:
    void set_shader_vp() {
        // TODO: move into renderer/scene?
        // this needs to come from the ECS somehow...
        auto& ecs = app.get_ecs();
        auto& cam_component = ecs.get_component<lv::ecs::Camera>(cam);
        shader->set_mat4("ViewProjection", cam_component.camera->get_vp_matrix());
    }

    void register_components() {
        auto& ecs = app.get_ecs();
        ecs.register_component<CameraController2D>();
        ecs.register_component<RotateOverTime>();
    }

    void register_systems() {
        auto& ecs = app.get_ecs();
        camera_controller_system = ecs.register_system<CameraController2DSystem>(CameraController2DSystem::get_archetype(ecs), ecs);
        rotate_over_time_system = ecs.register_system<RotateOverTimeSystem>(RotateOverTimeSystem::get_archetype(ecs), ecs);
    }

    void init_entities() {
        init_cam_entity();
        init_quad_entity();
        init_tri_entity();
    }

    void init_cam_entity() {
        auto camera = lv::Camera::make_orthographic(1.0f, app.get_window().get_aspect_ratio());
        auto& ecs = app.get_ecs();
        ecs.add_component<lv::ecs::Transform>(cam, {});
        ecs.add_component<lv::ecs::Camera>(cam, { camera });
        ecs.add_component<CameraController2D>(cam, {});
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
        ecs.add_component(quad, lv::ecs::Transform { glm::vec3(0), glm::identity<glm::quat>(), glm::vec3(5) });
        ecs.add_component(quad, lv::ecs::Mesh { material, vertex_array });
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
        ecs.add_component(tri, lv::ecs::Transform {});
        ecs.add_component(tri, lv::ecs::Mesh { mat, vertex_array });
        ecs.add_component(tri, RotateOverTime {});
    }

    bool handle(lv::Event const& event) override {
        switch (event.type) {
            case lv::Event::Type::WindowResized:
                app.get_ecs().get_component<lv::ecs::Camera>(cam).camera->set_aspect_ratio(app.get_window().get_aspect_ratio());
                set_shader_vp();
                break;
            case lv::Event::Type::MouseScrolled:
                app.get_ecs().get_component<CameraController2D>(cam).zoom_level += static_cast<int>(event.mouse.coord.y);
                break;
        }
        return false;
    }

    void update() override {
        rotate_over_time_system->update();
        camera_controller_system->update();
        set_shader_vp();
    }

    void gui() override {
        auto const dt = lv::time::render_delta_time();

        ImGui::Begin("Sandbox");
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

    lv::ecs::Entity quad, tri, cam;

    lv::ref<CameraController2DSystem> camera_controller_system;
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