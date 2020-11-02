#include "leviathan/lvpch.h"
#include "leviathan/ecs/system_manager.h"

namespace lv {
    namespace ecs {
        void SystemManager::on_entity_unmade(Entity entity) noexcept {
            for (auto const& kv : systems) {
                kv.second->remove(entity);
            }
        }

        void SystemManager::on_entity_archetype_changed(Entity entity, Archetype archetype) noexcept {
            for (auto const& kv : systems) {
                auto const& type = kv.first;
                auto const& system = kv.second;
                auto const& system_archetype = archetypes[type];

                if ((archetype & system_archetype) == system_archetype) {
                    system->add(entity);
                } else {
                    system->remove(entity);
                }
            }
        }
    }
}