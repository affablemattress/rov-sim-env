#pragma once

#include "config.hpp"

#include "gameobjects/components/texture.hpp"

#include "glad/glad.h"

namespace renderer {
    namespace texture {
        void useTexture2D(component::texture::Data& texture, GLuint imageUnitIndex);
    }
}