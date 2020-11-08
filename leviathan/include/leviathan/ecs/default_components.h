#pragma once

#include "ecs.h"
#include "components/camera.h"
#include "components/mesh.h"
#include "components/transform.h"

namespace lv {
    namespace ecs {
        void register_default_components(ECS& ecs);
    }
}
