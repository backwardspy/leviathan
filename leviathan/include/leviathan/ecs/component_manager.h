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
            template<class T> Component get_component_type() noexcept;
            template<class T> void add_component(Entity, T) noexcept;
            template<class T> void remove_component(Entity) noexcept;
            template<class T> T& get_component(Entity) noexcept;
            void on_entity_unmade(Entity) noexcept;

        private:
            template<class T>
            ComponentArray<T>& get_component_array() noexcept;

        private:
            using ComponentType = size_t;

            template<class T> static constexpr ComponentType component_type() { return static_cast<ComponentType>(typeid(T).hash_code()); }
            template<class T> static constexpr const char* component_name() { return typeid(T).name(); }

            std::unordered_map<ComponentType, Component> components {};
            std::unordered_map<ComponentType, std::unique_ptr<IComponentArray>> component_arrays {};
            Component next_component { 0 };
        };

        template<class T>
        inline void ComponentManager::register_component() noexcept {
            auto type = component_type<T>();
            auto name = component_name<T>();

            if (components.find(type) != std::end(components)) {
                Log::core_warn("Attempt to register component {} more than once.", name);
                return;
            }

            components[type] = next_component++;
            component_arrays[type] = std::make_unique<ComponentArray<T>>();

            Log::core_debug("Registered component {}.", name);
        }

        template<class T>
        inline Component ComponentManager::get_component_type() noexcept {
            auto type = component_type<T>();
            auto it = components.find(type);
            if (it == std::end(components)) {
                Log::core_warn("Attempt to get/add component {} before registering it. Component will be automatically registered.", component_name<T>());
                register_component<T>();
                return components.at(type);
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
        inline T& ComponentManager::get_component(Entity entity) noexcept {
            return get_component_array<T>().get(entity);
        }

        template<class T>
        inline ComponentArray<T>& ComponentManager::get_component_array() noexcept {
            auto type = component_type<T>();
            auto name = component_name<T>();
            auto it = component_arrays.find(type);
            if (it == std::end(component_arrays)) {
                Log::core_warn("Attempt to get/add component \"{}\" before registering it. Component will be automatically registered.", name);
                register_component<T>();
                return static_cast<ComponentArray<T>&>(*component_arrays.at(type));
            } else {
                return static_cast<ComponentArray<T>&>(*it->second);
            }
        }
    }
}
