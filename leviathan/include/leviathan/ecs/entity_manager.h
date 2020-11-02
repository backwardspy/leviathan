#pragma once

#include "leviathan/lvpch.h"
#include "entity.h"
#include "component.h"

namespace lv {
    namespace ecs {
        class EntityManager {
        public:
            EntityManager() noexcept;

            Entity make_entity();
            void unmake_entity(Entity);
            void set_archetype(Entity entity, Archetype archetype) noexcept;
            Archetype get_archetype(Entity entity) const noexcept;

        private:
            std::queue<Entity> available_entities {};
            std::vector<Archetype> archetypes { MaxEntities };
            size_t entity_count { 0 };
        };
    }
}
