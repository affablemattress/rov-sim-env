#pragma once

#include "config.hpp"

#include "glad/glad.h"

#include <string>

namespace component {
}

namespace renderer {
    struct Texture2D {
        GLuint textureBufferID;
        std::string filepath;

        Texture2D(const std::string& filepath);
    };

    void useTexture2D(renderer::Texture2D& texture, GLuint imageUnitIndex);
}