#include "leviathan/lvpch.h"
#include "leviathan/ecs/systems/mesh_renderer.h"
#include "leviathan/ecs/components/transform.h"
#include "leviathan/ecs/components/mesh.h"
#include "leviathan/renderer/renderer.h"

namespace lv {
    namespace ecs {
        MeshRenderer::MeshRenderer(ecs::ECS& ecs) : ecs { ecs } {}

        void MeshRenderer::render() const noexcept {
            for (const auto& entity : entities) {
                const auto& transform = ecs.get_component<lv::ecs::Transform>(entity);
                const auto& mesh = ecs.get_component<lv::ecs::Mesh>(entity);
                lv::Renderer::submit(*mesh.shader, *mesh.vertex_array, transform.calculate_matrix());
            }
        }

        Archetype MeshRenderer::get_archetype(ECS& ecs) noexcept {
            lv::ecs::Archetype archetype;
            archetype.set(ecs.get_component_type<lv::ecs::Transform>());
            archetype.set(ecs.get_component_type<lv::ecs::Mesh>());
            return archetype;
        }
    }
}