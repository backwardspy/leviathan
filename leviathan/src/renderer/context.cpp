#include "leviathan/lvpch.h"
#include "leviathan/renderer/context.h"

namespace lv {
    std::shared_ptr<lv::Material> Context::make_material(std::shared_ptr<Shader> shader) {
        return std::make_shared<lv::Material>(shader);
    }
}