#pragma once

// https://wiggling-bits.net/using-enum-classes-as-type-safe-bitmasks/

#include "lvpch.h"

namespace lv {
    template<typename Enum>
    struct EnableBitMaskOperators {
        static bool const enable = false;
    };
#define LV_ENABLE_BITMASK_OPERATORS(x) template<> struct EnableBitMaskOperators<x> { static bool const enable = true; };

    template<class Enum>
    typename std::enable_if<EnableBitMaskOperators<Enum>::enable, Enum>::type operator |(Enum lhs, Enum rhs) {
        static_assert(std::is_enum<Enum>::value, "template parameter is not an enum type");

        using underlying = typename std::underlying_type<Enum>::type;

        return static_cast<Enum> (
            static_cast<underlying>(lhs) |
            static_cast<underlying>(rhs)
            );
    }

    template<class Enum>
    typename std::enable_if<EnableBitMaskOperators<Enum>::enable, Enum>::type operator &(Enum lhs, Enum rhs) {
        static_assert(std::is_enum<Enum>::value, "template parameter is not an enum type");

        using underlying = typename std::underlying_type<Enum>::type;

        return static_cast<Enum> (
            static_cast<underlying>(lhs) &
            static_cast<underlying>(rhs)
            );
    }
}
