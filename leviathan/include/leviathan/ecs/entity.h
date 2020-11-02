#pragma once

#include "leviathan/lvpch.h"

namespace lv {
    namespace ecs {
        using Entity = std::uint64_t;

#if defined(DEBUG)
        // memory checks in debug mode makes checking these vectors pretty slow
        // so the max entities is lower
        constexpr size_t MaxEntities = 100000;
#else
        constexpr size_t MaxEntities = 1000000;
#endif
    }
}
