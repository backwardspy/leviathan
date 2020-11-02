#include "leviathan/lvpch.h"
#include "leviathan/ecs/component_manager.h"

namespace lv {
    namespace ecs {
        void ComponentManager::on_entity_unmade(Entity entity) noexcept {
            for (auto const& kv : component_arrays) {
                kv.second->on_entity_destroyed(entity);
            }
        }
    }
}