#pragma once

namespace lv {
    class Shader {
    public:
        enum class Type { Pixel, Vertex };

        // maps a shader source string to a shader type
        using SourceMap = std::unordered_map<Type, std::string const&>;

        virtual void use() = 0;

        virtual void set_mat4(std::string const& name, glm::mat4 mat) = 0;
        virtual void set_vec4(std::string const& name, glm::vec4 mat) = 0;

        virtual ~Shader() = default;
    };
}
