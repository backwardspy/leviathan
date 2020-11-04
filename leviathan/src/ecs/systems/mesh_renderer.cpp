#include "leviathan/lvpch.h"
#include "leviathan/ecs/systems/mesh_renderer.h"
#include "leviathan/ecs/components/transform.h"
#include "leviathan/ecs/components/mesh.h"
#include "leviathan/renderer/renderer.h"

namespace lv {
    namespace ecs {
        MeshRenderer::MeshRenderer(ecs::ECS& ecs) : ecs { ecs } {}

        void MeshRenderer::render() const {
            for (auto const& entity : entities) {
                auto const& transform = ecs.get_component<lv::ecs::Transform>(entity);
                auto const& mesh = ecs.get_component<lv::ecs::Mesh>(entity);
                lv::Renderer::submit(*mesh.material, *mesh.vertex_array, transform.calculate_matrix());
            }
        }

        Archetype MeshRenderer::get_archetype(ECS& ecs) {
            lv::ecs::Archetype archetype;
            archetype.set(ecs.get_component_type<lv::ecs::Transform>());
            archetype.set(ecs.get_component_type<lv::ecs::Mesh>());
            return archetype;
        }
    }
}