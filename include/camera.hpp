#pragma once

#include "config.hpp"

#include "glm/glm.hpp"

namespace camera {
    struct Object {
        float framebufferClearColor[4];
        glm::vec3 position;
        glm::vec2 rotation;
        float fov;
        int16_t framebufferWidth;
        int16_t framebufferHeight;
    };

    glm::mat4 buildViewMatrix(const camera::Object& camera);
    glm::mat4 buildProjectionMatrix(const camera::Object& camera);
}