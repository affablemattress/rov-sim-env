#pragma once

#include "config.hpp"

#include "glm/glm.hpp"

namespace camera {
    extern glm::vec3 cameraPosition;
    extern float fov;

    extern glm::mat4 viewMatrix;
    extern glm::mat4 projectionMatrix;

    void rebuildViewMatrix();
    void rebuildProjectionMatrix();
}