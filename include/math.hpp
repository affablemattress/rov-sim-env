#pragma once

#include "config.hpp"

#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"

namespace math {
    using TransformMatrix = glm::mat4;

    glm::mat4 buildTransformationMatrix(const float* position, const float* rotation, const float* scale);
    glm::mat4 buildNormalMatrixFromModelMatrix(const glm::mat4& modelMatrix);

    glm::vec3 getDirectionVectorFrom2DEuler(const glm::vec2& euler);

    static const float zeroes3[3] = { 0.f, 0.f, 0.f };
    static const float ones3[3] = { 0.f, 0.f, 0.f };
    static math::TransformMatrix identityTransformMatrix = buildTransformationMatrix(math::zeroes3, math::zeroes3, math::ones3);
}