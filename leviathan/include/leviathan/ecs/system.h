#pragma once

#include "leviathan/lvpch.h"
#include "entity.h"

namespace lv {
    namespace ecs {
        class System {
        public:
            const void add(Entity entity) { entities.insert(entity); }
            const void remove(Entity entity) { entities.erase(entity); }

            virtual ~System() noexcept = default;

        protected:
            std::set<Entity> entities;
        };
    }
}
