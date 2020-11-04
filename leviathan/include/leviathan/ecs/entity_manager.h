#pragma once

#include "leviathan/lvpch.h"
#include "entity.h"
#include "component.h"

namespace lv {
    namespace ecs {
        class EntityManager {
        public:
            EntityManager();

            Entity make_entity();
            void unmake_entity(Entity);
            void set_archetype(Entity entity, Archetype const& archetype);
            Archetype get_archetype(Entity entity) const;

        private:
            std::queue<Entity> available_entities {};
            std::vector<Archetype> archetypes = std::vector<Archetype>(MaxEntities);
            size_t entity_count = 0;
        };
    }
}
