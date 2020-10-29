#pragma once

namespace lv {
    class Buffer {
    public:
        virtual void use() noexcept = 0;

        virtual ~Buffer() {}
    };

    class VertexBuffer : public Buffer {
    public:
        virtual ~VertexBuffer() {}
    };

    class IndexBuffer :public Buffer {
    public:
        virtual size_t get_element_count() const noexcept = 0;

        virtual ~IndexBuffer() {}
    };
}
