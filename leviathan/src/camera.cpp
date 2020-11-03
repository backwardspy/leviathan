#include "leviathan/lvpch.h"

#include <glm/ext.hpp>

#include "leviathan/camera.h"
#include "leviathan/exc.h"
#include "leviathan/log.h"

namespace lv {
    Camera Camera::make_orthographic(float ortho_size, float aspect_ratio) {
        Camera cam { Camera::Mode::Orthographic, aspect_ratio };
        cam.ortho_size = ortho_size;
        cam.calculate_p_matrix();
        return cam;
    }

    Camera::Camera(Camera::Mode mode, float aspect_ratio) noexcept :
        mode { mode },
        aspect_ratio { aspect_ratio } {
        calculate_v_matrix();
    }

    void Camera::set_position(glm::vec3 position) noexcept {
        this->position = position;
        calculate_v_matrix();
    }

    void Camera::translate(glm::vec3 translation) noexcept {
        position += translation;
        calculate_v_matrix();
    }

    void Camera::set_aspect_ratio(float aspect_ratio) {
        this->aspect_ratio = aspect_ratio;
        calculate_p_matrix();
    }

    float Camera::get_ortho_size() const noexcept {
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
            default:
                throw exc::EnumeratorNotImplementedError {};
        }
    }

    void Camera::calculate_v_matrix() noexcept {
        v = glm::translate(glm::identity<glm::mat4>(), -position);
    }
}