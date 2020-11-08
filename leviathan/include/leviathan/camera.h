#pragma once

#include "lvpch.h"

namespace lv {
    class Camera {
    public:
        enum class Mode { Orthographic, Perspective };

        static ref<Camera> make_orthographic(float ortho_size, float aspect_ratio);
        static ref<Camera> make_perspective(float vertical_fov, float aspect_ratio, float near_plane = 0.1f, float far_plane = 100.0f);

        constexpr glm::mat4 get_v_matrix() const { return v; }
        constexpr glm::mat4 get_p_matrix() const { return p; }
        glm::mat4 get_vp_matrix() const { return p * v; }

        void set_position(glm::vec3 position);
        constexpr glm::vec3 get_position() const { return position; }
        void translate(glm::vec3 translation);

        constexpr float get_aspect_ratio() const { return aspect_ratio; }
        void set_aspect_ratio(float aspect_ratio);

        float get_ortho_size() const;
        void set_ortho_size(float size);

    private:
        Camera(Mode, float aspect_ratio);

        void calculate_p_matrix();
        void calculate_v_matrix();

    private:
        Mode mode;

        float aspect_ratio;

        glm::vec3 position { 0, 0, 0 };
        glm::mat4 v { glm::identity<glm::mat4>() };
        glm::mat4 p { glm::identity<glm::mat4>() };

        float ortho_size = 1.0f;

        float vertical_fov = 1.0f;
        float near_plane = 0.1f;
        float far_plane = 100.0f;
    };
}
