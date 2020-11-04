#pragma once

#include "leviathan/ecs/ecs.h"

namespace lv {
    namespace ecs {
        class MeshRenderer : public System {
        public:
            explicit MeshRenderer(ecs::ECS&);
            void render() const;

            static Archetype get_archetype(ECS& ecs);

        private:
            ecs::ECS& ecs;
        };
    }
}
