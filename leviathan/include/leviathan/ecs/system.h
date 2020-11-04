#pragma once

#include "leviathan/lvpch.h"
#include "entity.h"

namespace lv {
    namespace ecs {
        class System {
        public:
            void add(Entity entity) { entities.insert(entity); }
            void remove(Entity entity) { entities.erase(entity); }

            virtual ~System() = default;

        protected:
            std::set<Entity> entities;
        };
    }
}
