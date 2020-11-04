#include "leviathan/lvpch.h"
#include "leviathan/renderer/material.h"

namespace lv {
    Material::Material(std::shared_ptr<Shader> shader) : shader(shader) {}

    void Material::use() {
        for (auto const& param : params) {
            param.second->apply(param.first, *shader);
        }
        shader->use();
    }
}