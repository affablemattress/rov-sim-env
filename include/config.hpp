#pragma once

#include <stdint.h>

#define PROJECT_PATH "/home/kayra/dev/cpp/rov-sim-env/"

#define GLFW_INCLUDE_NONE

#define STB_IMAGE_IMPLEMENTATION
#define STBI_ONLY_PNG

namespace cfg {
    namespace window {
        extern const uint16_t width;
        extern const uint16_t height;
        extern const float fov;
        extern const char* title;
    }
}