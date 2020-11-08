#pragma once

#include "leviathan/lvpch.h"

namespace lv {
    struct Transform {
        glm::vec3 position = glm::vec3(0);
        glm::quat rotation = glm::identity<glm::quat>();
        glm::vec3 scale = glm::vec3(1);

        glm::mat4 matrix() const {
            auto transform = glm::identity<glm::mat4>();
            transform = glm::translate(transform, position);
            transform *= glm::mat4_cast(rotation);
            transform = glm::scale(transform, scale);
            return transform;
        }

        glm::mat4 inverse_matrix() const {
            return glm::inverse(matrix());
        }
    };
}
