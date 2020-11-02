#pragma once

#include "leviathan/lvpch.h"

namespace lv {
    namespace ecs {
        using Component = std::uint64_t;
        constexpr size_t MaxComponents = 64; // per entity

        using Archetype = std::bitset<MaxComponents>;
    }
}
