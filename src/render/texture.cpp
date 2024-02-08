#include "render/texture.hpp"

#include "stb_image.h"

namespace texture {
    void loadTexture2D(const char* filepath, const GLint textureBufferID) {
        GLint textureX, textureY, textureChannels;

        stbi_uc* textureData = stbi_load(filepath, &textureX, &textureY, &textureChannels, 3);

        glBindTexture(GL_TEXTURE_2D, textureBufferID);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, textureX, textureY, 0, GL_RGB, GL_UNSIGNED_BYTE, textureData);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glGenerateMipmap(GL_TEXTURE_2D);
        stbi_image_free(textureData);
    }
}