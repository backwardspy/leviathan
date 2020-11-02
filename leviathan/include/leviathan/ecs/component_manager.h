#pragma once

#include "leviathan/lvpch.h"
#include "leviathan/log.h"
#include "leviathan/ecs/entity.h"
#include "leviathan/ecs/component.h"
#include "leviathan/ecs/component_array.h"

namespace lv {
    namespace ecs {
        class ComponentManager {
        public:
            template<class T> void register_component() noexcept;
            template<class T> Component get_component_type() const;
            template<class T> void add_component(Entity, T) noexcept;
            template<class T> void remove_component(Entity) noexcept;
            template<class T> T& get_component(Entity) const noexcept;
            void on_entity_unmade(Entity) noexcept;

        private:
            template<class T>
            ComponentArray<T>& get_component_array() const;

        private:
            using ComponentType = size_t;

            template<class T>
            static constexpr ComponentType component_type() { return static_cast<ComponentType>(typeid(T).hash_code()); }

            std::unordered_map<ComponentType, Component> components {};
            std::unordered_map<ComponentType, std::unique_ptr<IComponentArray>> component_arrays {};
            Component next_component { 0 };
        };

        template<class T>
        inline void ComponentManager::register_component() noexcept {
            auto type = component_type<T>();

            if (components.find(type) != std::end(components)) {
                Log::warn("Attempt to register component type \"{}\" more than once.", typeid(T).name());
                return;
            }

            components[type] = next_component++;
            component_arrays[type] = std::make_unique<ComponentArray<T>>();
        }

        template<class T>
        inline Component ComponentManager::get_component_type() const {
            auto type = component_type<T>();
            auto it = components.find(type);
            if (it == std::end(components)) {
                throw exc::ComponentNotFound {};
            }
            return it->second;
        }

        template<class T>
        inline void ComponentManager::add_component(Entity entity, T component) noexcept {
            get_component_array<T>().add(entity, component);
        }

        template<class T>
        inline void ComponentManager::remove_component(Entity entity) noexcept {
            get_component_array<T>().remove(entity);
        }

        template<class T>
        inline T& ComponentManager::get_component(Entity entity) const noexcept {
            return get_component_array<T>().get(entity);
        }

        template<class T>
        inline ComponentArray<T>& ComponentManager::get_component_array() const {
            auto type = component_type<T>();
            auto it = component_arrays.find(type);
            if (it == std::end(component_arrays)) {
                throw exc::ComponentNotFound {};
            }
            return (ComponentArray<T>&)(*it->second);
        }
    }
}
