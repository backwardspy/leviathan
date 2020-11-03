#include "leviathan/lvpch.h"
#include "leviathan/ecs/default_components.h"

namespace lv {
    namespace ecs {
        void register_default_components(ECS& ecs) noexcept {
            ecs.register_component<Transform>();
            ecs.register_component<Mesh>();
        }
    }
}