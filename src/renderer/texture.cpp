#include "renderer/texture.hpp"

#include "stb_image.h"

namespace renderer {
    namespace texture {
        void useTexture2D(component::texture::Data& texture, GLuint imageUnitIndex) {
            glActiveTexture(imageUnitIndex);
            glBindTexture(GL_TEXTURE_2D, texture.textureBufferID);
        }
    }
}