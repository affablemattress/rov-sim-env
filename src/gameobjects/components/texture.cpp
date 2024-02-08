#include "gameobjects/components/texture.hpp"

#include "stb_image.h"
#include "spdlog/spdlog.h"

namespace component {
    namespace texture {
        Data::Data() : filepath(std::string_view()) {
                glGenTextures(1, &textureBufferID);
        }

        bool loadTexture2D(texture::Data& texture, std::string_view filepath) {
            if(texture.filepath.cbegin()) {
                spdlog::warn("Attempted to override loaded image {0} with {1}", texture.filepath.cbegin(), filepath.cbegin());
                return 0;
            }

            spdlog::info("Loading image (type: GL_TEXTURE_2D) at path: {0}", filepath.cbegin());

            GLint textureX, textureY, textureChannels;
            stbi_uc* textureData = stbi_load(filepath.cbegin(), &textureX, &textureY, &textureChannels, 3);

            glBindTexture(GL_TEXTURE_2D, texture.textureBufferID);

            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, textureX, textureY, 0, GL_RGB, GL_UNSIGNED_BYTE, textureData);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glGenerateMipmap(GL_TEXTURE_2D);
            stbi_image_free(textureData);
            return 1;
        }
    }
}