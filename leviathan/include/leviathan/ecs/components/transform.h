#pragma once

#include "leviathan/lvpch.h"

namespace lv {
    namespace ecs {
        struct Transform {
            glm::vec3 translation = glm::vec3(0);
            glm::quat rotation = glm::identity<glm::quat>();
            glm::vec3 scale = glm::vec3(1);

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
