#pragma once

namespace lv {
    template<class T>
    T remap(T v, T min_a, T max_a, T min_b, T max_b) {
        return min_b + (v - min_a) * (max_b - min_b) / (max_a - min_a);
    }
}
