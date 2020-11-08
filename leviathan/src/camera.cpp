#include "leviathan/lvpch.h"

#include <glm/ext.hpp>

#include "leviathan/camera.h"

namespace lv {
    ref<Camera> Camera::make_orthographic(float ortho_size, float aspect_ratio) {
        auto cam = ref<Camera>(new Camera(Camera::Mode::Orthographic, aspect_ratio));   // private constructor
        cam->ortho_size = ortho_size;
        cam->calculate_p_matrix();
        return cam;
    }

    ref<Camera> Camera::make_perspective(float vertical_fov, float aspect_ratio, float near_plane, float far_plane) {
        auto cam = ref<Camera>(new Camera(Camera::Mode::Perspective, aspect_ratio));    // private constructor
        cam->vertical_fov = vertical_fov;
        cam->near_plane = near_plane;
        cam->far_plane = far_plane;
        cam->calculate_p_matrix();
        return cam;
    }

    Camera::Camera(Camera::Mode mode, float aspect_ratio) :
        mode { mode },
        aspect_ratio { aspect_ratio } {
        calculate_v_matrix();
    }

    void Camera::set_position(glm::vec3 position) {
        this->position = position;
        calculate_v_matrix();
    }

    void Camera::translate(glm::vec3 translation) {
        position += translation;
        calculate_v_matrix();
    }

    void Camera::set_aspect_ratio(float aspect_ratio) {
        this->aspect_ratio = aspect_ratio;
        calculate_p_matrix();
    }

    float Camera::get_ortho_size() const {
        if (mode != Camera::Mode::Orthographic) {
            Log::core_warn("get_ortho_size called on non-orthographic camera. This value is likely meaningless.");
        }
        return ortho_size;
    }

    void Camera::set_ortho_size(float size) {
        if (mode != Camera::Mode::Orthographic) {
            Log::core_warn("set_ortho_size called on non-orthographic camera. This operation is likely meaningless.");
        }
        ortho_size = size;
        calculate_p_matrix();
    }

    void Camera::calculate_p_matrix() {
        switch (mode) {
            case Mode::Orthographic:
                p = glm::ortho(-ortho_size * aspect_ratio, ortho_size * aspect_ratio, -ortho_size, ortho_size);
                break;
            case Mode::Perspective:
                p = glm::perspective(vertical_fov, aspect_ratio, near_plane, far_plane);
                break;
            default:
                throw exc::EnumeratorNotImplemented {};
        }
    }

    void Camera::calculate_v_matrix() {
        v = glm::translate(glm::identity<glm::mat4>(), -position);
    }
}