#include "leviathan/lvpch.h"

#include "stb/stb_image.h"

#include "leviathan/platform/opengl/renderer/texture.h"

namespace lv {
    namespace opengl {
        Texture::Texture(std::string const& filename) {
            int width, height, channels;
            uint8_t* pixels = stbi_load(filename.c_str(), &width, &height, &channels, 4);

            if (!pixels) {
                Log::core_error("Failed to load texture from {}.", filename);
                throw exc::FileReadError {};
            }

            if (channels != 4) {
                // the best kind of warning - the engine provides no capability to fix this yet :)
                Log::core_warn("Loaded {} as a 4 channel texture but file only contains 3 data channels. Consider using RGB instead of RGBA for this texture.", filename);
            }

            glCreateTextures(GL_TEXTURE_2D, 1, &texture);
            glTextureParameteri(texture, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTextureParameteri(texture, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTextureParameteri(texture, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTextureParameteri(texture, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTextureStorage2D(texture, 8, GL_RGBA8, width, height); // TODO: calculate number of levels based on texture size
            glTextureSubImage2D(texture, 0, 0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
            glGenerateTextureMipmap(texture);

            stbi_image_free(pixels);
        }

        void Texture::use(size_t slot) {
            glBindTextureUnit(static_cast<GLuint>(slot), texture);
        }
    }
}