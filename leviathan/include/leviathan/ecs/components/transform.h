#pragma once

#include "leviathan/lvpch.h"

namespace lv {
    namespace ecs {
        struct Transform {
            glm::vec3 translation { 0.0f };
            glm::quat rotation { glm::identity<glm::quat>() };
            glm::vec3 scale { 1.0f };

            glm::mat4 calculate_matrix() const {
                auto transform = glm::identity<glm::mat4>();
                transform = glm::translate(transform, translation);
                transform *= glm::mat4_cast(rotation);
                transform = glm::scale(transform, scale);
                return transform;
            }
        };
    }
}
