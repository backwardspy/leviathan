#include "leviathan/lvpch.h"
#include "leviathan/renderer/material.h"

namespace lv {
    Material::Material(ref<Shader> shader) :
        shader(shader),
        textures(32, nullptr)    // reserve a little space for textures to avoid too many allocations later
    {}

    void Material::use() {
        for (auto const& param : params) {
            param.second->apply(param.first, *shader);
        }
        shader->use();

        for (size_t i = 0; i < textures.size(); ++i) {
            auto const& texture = textures[i];
            if (texture) texture->use(i);
        }
    }

    void Material::set_texture(std::string const& name, int32_t slot, ref<Texture> texture) {
        if (slot > textures.size()) {
            textures.resize(slot);
        }
        textures[slot] = texture;
        set_parameter(name, slot);
    }
}