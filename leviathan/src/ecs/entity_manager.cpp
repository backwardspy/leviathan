#include "leviathan/lvpch.h"
#include "leviathan/exc.h"
#include "leviathan/log.h"
#include "leviathan/ecs/entity.h"
#include "leviathan/ecs/entity_manager.h"

namespace lv {
    namespace ecs {
        EntityManager::EntityManager() noexcept {
            for (Entity ent = 0; ent < MaxEntities; ++ent) {
                available_entities.push(ent);
            }
        }

        Entity EntityManager::make_entity() {
            if (entity_count >= MaxEntities) {
                throw exc::EntityLimitReached {};
            }

            auto entity = available_entities.front();
            available_entities.pop();
            ++entity_count;

            return entity;
        }

        void EntityManager::unmake_entity(Entity entity) {
            if (entity >= MaxEntities) {
                Log::error("unmake: Entity #{} is out of range.", entity);
                return;
            }

            auto& archetype = archetypes[static_cast<size_t>(entity)];

            if (!archetype.any()) {
                Log::error("unmake: Entity #{} is already inactive.", entity);
                return;
            }

            // no components - entity is effectively dead
            archetype.reset();
            available_entities.push(entity);
            --entity_count;
        }

        void EntityManager::set_archetype(Entity entity, Archetype archetype) noexcept {
            archetypes[static_cast<size_t>(entity)] = archetype;
        }

        Archetype EntityManager::get_archetype(Entity entity) const noexcept {
            return archetypes[static_cast<size_t>(entity)];
        }
    }
}