#pragma once

#include "config.hpp"

#include "glad/glad.h"

#include <string>

namespace component {
    namespace texture {
        struct Data {
            GLuint textureBufferID;
            std::string_view filepath;

            Data();
        };

        bool loadTexture2D(texture::Data& texture, std::string_view filepath);
    }
}