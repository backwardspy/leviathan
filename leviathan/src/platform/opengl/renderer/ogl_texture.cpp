#include "leviathan/lvpch.h"

#include "stb/stb_image.h"

#include "leviathan/platform/opengl/renderer/texture.h"

namespace lv {
    namespace opengl {
        Texture::Texture(std::string const& filename) {
            int width, height, channels;
            uint8_t* pixels = stbi_load(filename.c_str(), &width, &height, &channels, 0);

            if (!pixels) {
                Log::core_error("Failed to load texture from {}.", filename);
                throw exc::FileReadError {};
            }

            GLenum image_fmt, storage_fmt;

            switch (channels) {
                case 3:
                    image_fmt = GL_RGB;
                    storage_fmt = GL_RGB8;
                    break;
                case 4:
                    image_fmt = GL_RGBA;
                    storage_fmt = GL_RGBA8;
                    break;
                default:
                    Log::core_error("Texture {} has unsupported number of channels: {}", filename, channels);
                    throw exc::FileReadError {};
            }

            glCreateTextures(GL_TEXTURE_2D, 1, &texture);
            glTextureParameteri(texture, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTextureParameteri(texture, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTextureParameteri(texture, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTextureParameteri(texture, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTextureStorage2D(texture, 8, storage_fmt, width, height); // TODO: calculate number of levels based on texture size
            glTextureSubImage2D(texture, 0, 0, 0, width, height, image_fmt, GL_UNSIGNED_BYTE, pixels);
            glGenerateTextureMipmap(texture);

            stbi_image_free(pixels);

            Log::core_debug("Loaded {} as {} channel 2D texture.", filename, channels);
        }

        void Texture::use(size_t slot) {
            glBindTextureUnit(static_cast<GLuint>(slot), texture);
        }
    }
}