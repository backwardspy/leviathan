#include "leviathan/lvpch.h"
#include "leviathan/ecs/ecs.h"

namespace lv {
    namespace ecs {
        Entity ECS::make_entity() {
            return entity_man.make_entity();
        }

        void ECS::unmake_entity(Entity entity) {
            entity_man.unmake_entity(entity);
            component_man.on_entity_unmade(entity);
            system_man.on_entity_unmade(entity);
        }

        void ECS::update_archetype(Entity entity, Component component, bool has_component) {
            auto archetype = entity_man.get_archetype(entity);
            archetype.set(component, has_component);
            entity_man.set_archetype(entity, archetype);
            system_man.on_entity_archetype_changed(entity, archetype);
        }
    }
}