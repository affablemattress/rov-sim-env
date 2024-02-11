#include "renderer/texture.hpp"

#include "stb_image.h"
#include "spdlog/spdlog.h"

#include <string>

namespace renderer {
    Texture2D::Texture2D(const std::string& filepath) {
        glGenTextures(1, &textureBufferID);
        this->filepath = filepath;

        spdlog::info("Loading image (type: GL_TEXTURE_2D) at path: {0}", this->filepath.c_str());

        GLint textureX, textureY, textureChannels;
        stbi_uc* textureData = stbi_load(this->filepath.c_str(), &textureX, &textureY, &textureChannels, 3);

        glBindTexture(GL_TEXTURE_2D, textureBufferID);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureX, textureY, 0, GL_RGB, GL_UNSIGNED_BYTE, textureData);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glGenerateMipmap(GL_TEXTURE_2D);
        stbi_image_free(textureData);
    }

    void useTexture2D(renderer::Texture2D& texture, GLuint imageUnitIndex) {
        glActiveTexture(imageUnitIndex);
        glBindTexture(GL_TEXTURE_2D, texture.textureBufferID);
    }
}