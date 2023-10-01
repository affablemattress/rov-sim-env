#pragma once

#include <stdint.h>

#define GLFW_INCLUDE_NONE
#define PROJECT_PATH "/home/kayra/dev/cpp/rov-sim-env/"

namespace cfg {
    namespace window {
        extern const uint16_t width;
        extern const uint16_t height;
        extern const char* title;
    }
}