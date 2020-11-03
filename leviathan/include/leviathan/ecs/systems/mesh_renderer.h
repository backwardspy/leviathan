#pragma once

#include "leviathan/ecs/ecs.h"

namespace lv {
    namespace ecs {
        class MeshRenderer : public System {
        public:
            MeshRenderer(ecs::ECS&);
            void render() const noexcept;

            static Archetype get_archetype(ECS& ecs) noexcept;

        private:
            ecs::ECS& ecs;
        };
    }
}
