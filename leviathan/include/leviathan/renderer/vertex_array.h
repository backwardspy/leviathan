#pragma once

namespace lv {
    class VertexArray {
    public:
        virtual void use() = 0;
        virtual size_t get_element_count() const = 0;

        virtual ~VertexArray() = default;
    };
}
