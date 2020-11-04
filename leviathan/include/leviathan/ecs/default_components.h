#pragma once

#include "ecs.h"
#include "components/transform.h"
#include "components/mesh.h"

namespace lv {
    namespace ecs {
        void register_default_components(ECS& ecs);
    }
}
