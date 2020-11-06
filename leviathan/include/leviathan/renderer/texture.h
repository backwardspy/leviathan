#pragma once

namespace lv {
    class Texture {
    public:
        virtual void use(size_t slot) = 0;

        virtual ~Texture() = default;
    };
}
