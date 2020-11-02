#pragma once

#include "system.h"
#include "entity.h"
#include "component.h"

namespace lv {
    namespace ecs {
        class SystemManager {
        public:
            template<class T, class... Args> std::shared_ptr<T> register_system(Archetype, Args&&...) noexcept;

            void on_entity_unmade(Entity) noexcept;
            void on_entity_archetype_changed(Entity, Archetype) noexcept;

        private:
            using SystemType = size_t;

            template<class T>
            static constexpr SystemType system_type() { return static_cast<SystemType>(typeid(T).hash_code()); }

            std::unordered_map<SystemType, Archetype> archetypes {};
            std::unordered_map<SystemType, std::shared_ptr<System>> systems {};
        };

        template<class T, class... Args>
        inline std::shared_ptr<T> SystemManager::register_system(Archetype archetype, Args&&... args) noexcept {
            auto type = system_type<T>();
            auto system = std::make_shared<T>(std::forward<Args>(args)...);
            systems[type] = system;
            return system;
        }
    }
}
