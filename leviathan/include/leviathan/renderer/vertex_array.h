#pragma once

namespace lv {
    class VertexArray {
    public:
        virtual void use() noexcept = 0;
        virtual size_t get_element_count() const noexcept = 0;

        virtual ~VertexArray() {}
    };
}
