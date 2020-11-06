#pragma once

#include <glm/glm.hpp>

namespace lv {
    enum class ElementType {
        Float, UInt
    };

    struct Element {
        ElementType type;
        unsigned int components;
        size_t size;
    };

    using ElementLayout = std::vector<Element>;

    template<class T>
    static ElementLayout element_layout() { static_assert(false, "element_layout is not implemented for this type"); }

    struct Vertex {
        glm::vec3 position = glm::vec3(0);
        glm::vec4 color = glm::vec4(1);
        glm::vec2 uv = glm::vec2(0);
    };

    template<>
    static ElementLayout element_layout<Vertex>() {
        return {
            { ElementType::Float, 3, sizeof(glm::vec3) },
            { ElementType::Float, 4, sizeof(glm::vec4) },
            { ElementType::Float, 2, sizeof(glm::vec2) },
        };
    }

    using Index = uint32_t;
}
