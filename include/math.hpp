#pragma once

#include "config.hpp"

#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"

namespace math {
    using TransformMatrix = glm::mat4;

    struct Transform {
        float position[3];
        float rotation[3];
        float scale[3];
    };

    glm::mat4 buildTransformMatrix(const float* position, const float* rotation, const float* scale);
    glm::mat4 buildTransformMatrix(const Transform* transform);

    glm::mat4 buildNormalMatrixFromTransformMatrix(const glm::mat4& modelMatrix);

    glm::vec3 getDirectionVectorFrom2DEuler(const glm::vec2& euler);

    static const float zeroes3[3] = { 0.f, 0.f, 0.f };
    static const float ones3[3] = { 1.f, 1.f, 1.f };
    static math::TransformMatrix identityTransformMatrix = glm::mat4(1.f);
}