#pragma once 

#include "config.hpp"

#include "glm/glm.hpp"

extern glm::mat4 projectionMatrix;
extern int16_t windowWidth;
extern int16_t windowHeight;

namespace common {
    extern int16_t framebufferWidth;
    extern int16_t framebufferHeight;
    extern float fov;
}