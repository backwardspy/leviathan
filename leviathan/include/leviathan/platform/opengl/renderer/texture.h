#pragma once

#include "leviathan/renderer/texture.h"
#include "leviathan/platform/opengl/opengl.h"

namespace lv {
    namespace opengl {
        class Texture : public lv::Texture {
        public:
            explicit Texture(std::string const& filename);

            void use(size_t slot) override;

        private:
            GLuint texture;
        };
    }
}
