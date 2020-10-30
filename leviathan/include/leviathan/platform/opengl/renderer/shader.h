#pragma once

#include "leviathan/lvpch.h"
#include "leviathan/platform/opengl/opengl.h"
#include "leviathan/renderer/shader.h"

namespace lv {
    namespace opengl {
        class Shader : public lv::Shader {
        public:
            Shader(const lv::Shader::SourceMap& sources);

            virtual void use() noexcept override;

            ~Shader() noexcept;

        private:
            GLuint program;
        };
    }
}
