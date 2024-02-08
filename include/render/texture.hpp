#pragma once

#include "config.hpp"

#include "glad/glad.h"

namespace texture {
    void loadTexture2D(const char* filepath, const GLint textureBufferID);
}