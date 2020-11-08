#include "leviathan/lvpch.h"
#include "leviathan/scene/components/camera.h"

namespace lv {
    Camera Camera::make_orthographic(float ortho_size, float aspect_ratio) {
        auto cam = Camera(Camera::Mode::Orthographic, aspect_ratio);
        cam.ortho_size = ortho_size;
        return cam;
    }

    Camera Camera::make_perspective(float vertical_fov, float aspect_ratio, float near_plane, float far_plane) {
        auto cam = Camera(Camera::Mode::Perspective, aspect_ratio);
        cam.vertical_fov = vertical_fov;
        cam.near_plane = near_plane;
        cam.far_plane = far_plane;
        return cam;
    }

    Camera::Camera(Camera::Mode mode, float aspect_ratio) :
        mode { mode },
        aspect_ratio { aspect_ratio } {
    }

    glm::mat4 Camera::get_projection() const {
        switch (mode) {
            case Mode::Orthographic:
                return glm::ortho(-ortho_size * aspect_ratio, ortho_size * aspect_ratio, -ortho_size, ortho_size);
            case Mode::Perspective:
                return glm::perspective(vertical_fov, aspect_ratio, near_plane, far_plane);
            default:
                throw exc::EnumeratorNotImplemented {};
        }
    }

    glm::vec2 Camera::unproject(glm::vec2 ndc) const {
        return glm::inverse(get_projection()) * glm::vec4(ndc, 0, 1);
    }
}