#pragma once

namespace lv {
    class Shader {
    public:
        enum class Type { Pixel, Vertex };

        // maps a shader source string to a shader type
        using SourceMap = std::unordered_map<Type, const std::string&>;

        virtual void use() noexcept = 0;

        virtual ~Shader() noexcept {}
    };
}
