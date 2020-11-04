#pragma once

#include "system.h"
#include "entity.h"
#include "component.h"

namespace lv {
    namespace ecs {
        class SystemManager {
        public:
            template<class T, class... Args> std::shared_ptr<T> register_system(Archetype, Args&&...);

            void on_entity_unmade(Entity) const;
            void on_entity_archetype_changed(Entity, Archetype const&) const;

        private:
            using SystemType = size_t;

            template<class T> static SystemType system_type() { return static_cast<SystemType>(typeid(T).hash_code()); }
            template<class T> static char const* system_name() { return typeid(T).name(); }

            std::unordered_map<SystemType, Archetype> archetypes {};
            std::unordered_map<SystemType, std::shared_ptr<System>> systems {};
        };

        template<class T, class... Args>
        inline std::shared_ptr<T> SystemManager::register_system(Archetype archetype, Args&&... args) {
            auto type = system_type<T>();
            auto name = system_name<T>();

            if (systems.find(type) != std::end(systems)) {
                Log::core_warn("Attempt to register duplicate system {}.", name);
                return std::static_pointer_cast<T>(systems[type]);
            }

            auto system = std::make_shared<T>(std::forward<Args>(args)...);
            systems[type] = system;
            archetypes[type] = archetype;

            Log::core_debug("Registered system {}.", name);

            return system;
        }
    }
}
