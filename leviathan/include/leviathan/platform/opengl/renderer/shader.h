#pragma once

#include "leviathan/lvpch.h"
#include "leviathan/platform/opengl/opengl.h"
#include "leviathan/renderer/shader.h"

namespace lv {
    namespace opengl {
        class Shader : public lv::Shader {
        public:
            explicit Shader(const lv::Shader::SourceMap& sources);

            void use() noexcept override;

            ~Shader() noexcept override;

        private:
            GLuint program;
        };
    }
}
