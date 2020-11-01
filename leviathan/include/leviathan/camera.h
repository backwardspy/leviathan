#pragma once

#include "lvpch.h"

namespace lv {
    class Camera {
    public:
        enum class Mode { Orthographic };

        static Camera make_orthographic(float ortho_size, float aspect_ratio);

        constexpr glm::mat4 get_v_matrix() const noexcept { return v; }
        constexpr glm::mat4 get_p_matrix() const noexcept { return p; }
        glm::mat4 get_vp_matrix() const noexcept { return p * v; }

        void set_position(glm::vec3 position) noexcept;
        constexpr glm::vec3 get_position() const noexcept { return position; }
        void translate(glm::vec3 translation) noexcept;

        constexpr float get_aspect_ratio() const noexcept { return aspect_ratio; }
        void set_aspect_ratio(float aspect_ratio);

        float get_ortho_size() const noexcept;
        void set_ortho_size(float size);

    private:
        Camera(Mode, float aspect_ratio) noexcept;

        void calculate_p_matrix();
        void calculate_v_matrix() noexcept;

    private:
        Mode mode;

        float aspect_ratio;

        glm::vec3 position { 0, 0, 0 };
        glm::mat4 v, p;

        float ortho_size = 1.0f;
    };
}
