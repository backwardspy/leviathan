#include "leviathan/lvpch.h"
#include "leviathan/ecs/default_components.h"

namespace lv {
    namespace ecs {
        void register_default_components(ECS& ecs) {
            ecs.register_component<Camera>();
            ecs.register_component<Mesh>();
            ecs.register_component<Transform>();
        }
    }
}