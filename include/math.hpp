#pragma once

#include "config.hpp"

#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"

namespace math {
    glm::mat4 buildModelMatrix(const float* position, const float* rotation, const float* scale);
    glm::mat3 buildNormalMatrixFromModelMatrix(const glm::mat4& modelMatrix);

    glm::vec3 getDirectionVectorFrom2DEuler(const glm::vec2& euler);
}