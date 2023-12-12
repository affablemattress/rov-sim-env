#pragma once

#include "config.hpp"

#include "glm/glm.hpp"

namespace camera {
    struct object {
        float framebufferClearColor[4];
        glm::vec3 position;
        float fov;
        int16_t framebufferWidth;
        int16_t framebufferHeight;
    };

    glm::mat4 buildViewMatrix(const camera::object& camera);
    glm::mat4 buildProjectionMatrix(const camera::object& camera);
}