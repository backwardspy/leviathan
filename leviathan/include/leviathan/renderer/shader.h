#pragma once

namespace lv {
    class Shader {
    public:
        enum class Type { Pixel, Vertex };

        virtual void use() = 0;

        virtual void set_mat4(std::string const& name, glm::mat4) = 0;
        virtual void set_vec4(std::string const& name, glm::vec4) = 0;
        virtual void set_int(std::string const& name, int32_t) = 0;
        virtual void set_float(std::string const& name, float) = 0;

        void set_alpha_blend(bool enabled) { options.alpha_blend = enabled; }

        virtual ~Shader() = default;

    protected:
        struct {
            bool alpha_blend = false;
        } options;
    };
}
