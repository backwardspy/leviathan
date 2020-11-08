#include <leviathan/leviathan.h>

class CustomLayer : public lv::Layer {
public:
    explicit CustomLayer(lv::Application& app) :
        Layer("Custom"),
        app(app),
        window(app.get_window()),
        ctx(app.get_render_context()),
        ent_registry(app.get_ent_registry()),
        camera(ent_registry.create()),
        duck(ent_registry.create()) {
        init_camera();
        init_ducks();
    }

private:
    bool handle(lv::Event const& event) override {
        switch (event.type) {
            case lv::Event::Type::WindowResized:
                ent_registry.get<lv::Camera>(camera).aspect_ratio = window.get_aspect_ratio();
                break;
            case lv::Event::Type::KeyPressed:
                if (event.key.code == lv::KeyCode::Space) {
                    zoom_level = 0;
                    auto& [cam, transform] = ent_registry.get<lv::Camera, lv::Transform>(camera);
                    cam.ortho_size = 1.0f;
                    transform.position = glm::vec3(0);
                }
                break;
            case lv::Event::Type::ButtonPressed:
                if (event.button.code == lv::ButtonCode::Right) dragging = true;
                break;
            case lv::Event::Type::ButtonReleased:
                if (event.button.code == lv::ButtonCode::Right) dragging = false;
                break;
            case lv::Event::Type::MouseMoved:
                if (dragging) drag_camera(event.mouse.delta);
                break;
            case lv::Event::Type::MouseScrolled:
                zoom_camera(static_cast<int>(event.mouse.delta.y));
                break;
        }
        return false;
    }

    void gui() override {
        auto const dt = lv::time::render_delta_time();

        ImGui::Begin("Sandbox");
        ImGui::Text("Performance: %.3f ms/frame (%.f fps)", lv::time::to_milliseconds(dt).count(), 1.0f / lv::time::to_seconds(dt).count());
        ImGui::Text("Simulation Time: %.3f", lv::time::to_seconds(lv::time::elapsed()).count());
        ImGui::End();

        ImGui::Begin("Shader");
        if (ImGui::ColorPicker4("Tint", glm::value_ptr(tint))) {
            ent_registry.get<lv::MeshRenderer>(duck).material->set_parameter("Tint", tint);
        }
        ImGui::End();
    }

    void drag_camera(glm::vec2 mouse_delta) {
        auto motion_ndc = 2.0f * mouse_delta / static_cast<glm::vec2>(window.get_size());
        // usually we'd flip the Y, but since we're subtracting this motion anyway we flip the x instead
        motion_ndc.x = -motion_ndc.x;

        auto& [cam, transform] = ent_registry.get<lv::Camera, lv::Transform>(camera);

        auto world_motion = cam.unproject(motion_ndc);
        transform.position += glm::vec3(world_motion, 0);
    }

    void zoom_camera(int zoom) {
        zoom_level += zoom;

        auto mouse_pos = lv::remap(
            lv::Input::get_mouse_position(),
            glm::vec2(0),
            static_cast<glm::vec2>(window.get_size()),
            glm::vec2(-1),
            glm::vec2(1)
        );
        mouse_pos.y = -mouse_pos.y;

        auto& [cam, transform] = ent_registry.get<lv::Camera, lv::Transform>(camera);

        auto pos_before = cam.unproject(mouse_pos);
        cam.ortho_size = pow(1.1f, -zoom_level);
        auto diff = pos_before - cam.unproject(mouse_pos);

        transform.position += glm::vec3(diff, 0);
    }

    void init_camera() {
        ent_registry.emplace<lv::Transform>(camera);
        ent_registry.emplace<lv::Camera>(camera, lv::Camera::make_orthographic(1.0f, window.get_aspect_ratio()));
    }

    void init_ducks() {
        auto shader = ctx.make_shader("assets/shaders/unlit_generic.glsl", { lv::Shader::Type::Vertex, lv::Shader::Type::Pixel });
        shader->set_alpha_blend(true);

        auto material = ctx.make_material(shader);
        material->set_texture("MainTex", 0, ctx.make_texture("assets/textures/duck.png"));
        material->set_parameter("Tint", tint);

        auto vertex_array = [&ctx = ctx] {
            return ctx.make_vertex_array(
                {
                    lv::Vertex{ glm::vec3(-0.5f,  0.5f, 0), glm::vec4(1), glm::vec2(0) },
                    lv::Vertex{ glm::vec3(0.5f,  0.5f, 0), glm::vec4(1), glm::vec2(1, 0) },
                    lv::Vertex{ glm::vec3(-0.5f, -0.5f, 0), glm::vec4(1), glm::vec2(0, 1) },
                    lv::Vertex{ glm::vec3(0.5f, -0.5f, 0), glm::vec4(1), glm::vec2(1) },
                }, {
                    0, 1, 2,
                    1, 3, 2
                });
        }();

        ent_registry.emplace<lv::Transform>(duck);
        ent_registry.emplace<lv::MeshRenderer>(duck, material, vertex_array);
    }

private:
    lv::Application& app;
    lv::Window& window;
    lv::Context& ctx;
    entt::registry& ent_registry;

    entt::entity camera, duck;

    int zoom_level = 0;
    bool dragging = false;
    glm::vec2 drag_start {};

    // shader params
    glm::vec4 tint = glm::vec4(1.0f);
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