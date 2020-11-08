#pragma once

#include "leviathan/lvpch.h"

namespace lv {
    struct Camera {
        enum class Mode { Orthographic, Perspective };

        static Camera make_orthographic(float ortho_size, float aspect_ratio);
        static Camera make_perspective(float vertical_fov, float aspect_ratio, float near_plane = 0.1f, float far_plane = 100.0f);

        glm::mat4 get_projection() const;
        glm::vec2 unproject(glm::vec2 ndc) const;

        Mode mode;

        bool active = false;

        float aspect_ratio;

        float ortho_size = 1.0f;

        float vertical_fov = 1.0f;
        float near_plane = 0.1f;
        float far_plane = 100.0f;

    private:
        Camera(Mode, float aspect_ratio);
    };
}
