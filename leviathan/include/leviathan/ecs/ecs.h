#pragma once

#include "entity_manager.h"
#include "component_manager.h"
#include "system_manager.h"

namespace lv {
    namespace ecs {
        class ECS {
        public:
#pragma region Entity
            Entity make_entity() noexcept;
            void unmake_entity(Entity) noexcept;
#pragma endregion

#pragma region Component
            template<class T> void register_component() noexcept;
            template<class T> void add_component(Entity, T) noexcept;
            template<class T> void remove_component(Entity) noexcept;
            template<class T> T& get_component(Entity) const noexcept;
            template<class T> Component get_component_type() const noexcept;
#pragma endregion

#pragma region System
            template<class T, class... Args> std::shared_ptr<T> register_system(Archetype, Args&&...) noexcept;
#pragma endregion

        private:
            void update_archetype(Entity, Component, bool);

        private:
            EntityManager entity_man {};
            ComponentManager component_man {};
            SystemManager system_man {};
        };

        template<class T>
        inline void ECS::register_component() noexcept {
            component_man.register_component<T>();
        }

        template<class T>
        inline void ECS::add_component(Entity entity, T component) noexcept {
            component_man.add_component(entity, component);
            update_archetype(entity, component_man.get_component_type<T>(), true);
        }

        template<class T>
        inline void ECS::remove_component(Entity entity) noexcept {
            component_man.remove_component<T>(entity);
            update_archetype(entity, component_man.get_component_type<T>(), false);
        }

        template<class T>
        inline T& ECS::get_component(Entity entity) const noexcept {
            return component_man.get_component<T>(entity);
        }

        template<class T>
        inline Component ECS::get_component_type() const noexcept {
            return component_man.get_component_type<T>();
        }

        template<class T, class... Args>
        inline std::shared_ptr<T> ECS::register_system(Archetype archetype, Args&&... args) noexcept {
            return system_man.register_system<T>(archetype, std::forward<Args>(args)...);
        }
    }
}
