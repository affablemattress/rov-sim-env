#pragma once

#include "config.hpp"

#include "glm/glm.hpp"

namespace component {
    struct Transform {
        glm::vec3 position;
        glm::vec3 rotation;
        glm::vec3 scale;
    };
}