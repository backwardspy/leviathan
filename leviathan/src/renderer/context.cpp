#include "leviathan/lvpch.h"
#include "leviathan/renderer/context.h"

namespace lv {
    ref<lv::Material> Context::make_material(ref<Shader> shader) {
        return make_ref<lv::Material>(shader);
    }
}