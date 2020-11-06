#pragma once

#include "leviathan/lvpch.h"
#include "shader.h"
#include "texture.h"

namespace lv {
    class IMaterialParam {
    public:
        virtual void apply(std::string const& name, Shader& shader) const = 0;

        virtual ~IMaterialParam() = default;
    };

    template<class T>
    class MaterialParam;

    template<>
    class MaterialParam<glm::mat4> : public IMaterialParam {
    public:
        explicit MaterialParam(glm::mat4 const& value) : value(value) {}
        void apply(std::string const& name, Shader& shader) const override;

    public:
        glm::mat4 value;
    };

    template<>
    class MaterialParam<glm::vec4> : public IMaterialParam {
    public:
        explicit MaterialParam(glm::vec4 const& value) : value(value) {}
        void apply(std::string const& name, Shader& shader) const override;

    public:
        glm::vec4 value;
    };

    template<>
    class MaterialParam<float> : public IMaterialParam {
    public:
        explicit MaterialParam(float value) : value(value) {}
        void apply(std::string const& name, Shader& shader) const override;

    public:
        float value;
    };

    template<>
    class MaterialParam<int32_t> : public IMaterialParam {
    public:
        explicit MaterialParam(int32_t value) : value(value) {}
        void apply(std::string const& name, Shader& shader) const override;

    public:
        int32_t value;
    };

    class Material {
    public:
        explicit Material(ref<Shader>);

        void use();

        template<class T> void set_parameter(std::string const& name, T const& value);
        void set_texture(std::string const& name, int32_t slot, ref<Texture> texture);

        Shader const& get_shader() const { return *shader; }
        Shader& get_shader() { return *shader; }

    private:
        ref<Shader> shader;
        std::unordered_map<std::string, scope<IMaterialParam>> params;
        std::vector<ref<Texture>> textures;
    };

    template<class T>
    inline void Material::set_parameter(std::string const& name, T const& value) {
        if (params.find(name) == std::end(params)) {
            // TODO: CppCheck says searching before inserting here isn't necessary.
            // I'm inclined to disagree because we don't want to delete and recreate the object if it already exists.
            params[name] = make_scope<MaterialParam<T>>(value);   // cppcheck-suppress stlFindInsert
        }

        // TODO: is there some way we could validate the type here and static_cast?
        auto param_ptr = dynamic_cast<MaterialParam<T>*>(params[name].get());
        if (!param_ptr) {
            Log::core_error("set_parameter: attempted to set parameter \"{}\" with value of incorrect type {}", name, typeid(T).name());
            return;
        }
        param_ptr->value = value;
    }

    inline void MaterialParam<glm::mat4>::apply(std::string const& name, Shader& shader) const { shader.set_mat4(name, value); }
    inline void MaterialParam<glm::vec4>::apply(std::string const& name, Shader& shader) const { shader.set_vec4(name, value); }
    inline void MaterialParam<int32_t>::apply(std::string const& name, Shader& shader) const { shader.set_int(name, value); }
    inline void MaterialParam<float>::apply(std::string const& name, Shader& shader) const { shader.set_float(name, value); }
}
