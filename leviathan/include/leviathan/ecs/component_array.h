#pragma once

#include "entity.h"
#include "leviathan/log.h"

namespace lv {
    namespace ecs {
        class IComponentArray {
        public:
            virtual void on_entity_destroyed(Entity) = 0;

            virtual ~IComponentArray() = default;
        };

        template<class T>
        class ComponentArray : public IComponentArray {
        public:
            void add(Entity, T) noexcept;
            void remove(Entity);
            T& get(Entity);

            void on_entity_destroyed(Entity) override;

        private:
            std::vector<T> components { MaxEntities };
            std::unordered_map<Entity, size_t> entity_indices;
            std::unordered_map<size_t, Entity> index_entities;
            size_t size;
        };

        template<class T>
        inline void ComponentArray<T>::add(Entity entity, T component) noexcept {
            if (entity_indices.find(entity) != std::end(entity_indices)) {
                Log::warn("Attempt to add a duplicate component to entity #{}.", entity);
                return;
            }
            entity_indices[entity] = size;
            index_entities[size] = entity;
            components[size] = component;
            ++size;
        }

        template<class T>
        inline void ComponentArray<T>::remove(Entity entity) {
            if (entity_indices.find(entity) == std::end(entity_indices)) {
                Log::warn("Attempt to remove non-existent component from entity #{}", entity);
                return;
            }

            auto remove_idx = entity_indices[entity];
            auto last = size - 1;
            auto last_entity = index_entities[last];

            components[remove_idx] = components[last];  // move last component into cleared location
            entity_indices[last_entity] = remove_idx;   // move last entity into cleared location
            index_entities[remove_idx] = last_entity;   // ^

            entity_indices.erase(entity);   // remove entity<->index link
            index_entities.erase(last);     // ^

            --size;
        }

        template<class T>
        inline T& ComponentArray<T>::get(Entity entity) {
            auto it = entity_indices.find(entity);
            if (it == std::end(entity_indices)) {
                throw exc::EntityNotFound { };
            }

            return components[it->second];
        }

        template<class T>
        inline void ComponentArray<T>::on_entity_destroyed(Entity entity) {
            // TODO: validate it exists to suppress warnings?
            remove(entity);
        }
    }
}
