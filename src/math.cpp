#include "math.hpp"

namespace math {
    glm::mat4 buildModelMatrix(const float* position, const float* rotation, const float* scale) {
        glm::mat4 returnMatrix = glm::mat4(1.f);
        returnMatrix = glm::translate(returnMatrix, { position[0], position[1], position[2] });
        returnMatrix = glm::rotate(returnMatrix, glm::radians(rotation[0]), glm::vec3(1.f, 0.f, 0.f));
        returnMatrix = glm::rotate(returnMatrix, glm::radians(rotation[1]), glm::vec3(0.f, 1.f, 0.f));
        returnMatrix = glm::rotate(returnMatrix, glm::radians(rotation[2]), glm::vec3(0.f, 0.f, 1.f));
        returnMatrix = glm::scale(returnMatrix, { scale[0], scale[1], scale[2] });
        return returnMatrix;
    }

    glm::vec3 getDirectionVectorFrom2DEuler(const glm::vec2& euler) {
        double x = glm::radians(euler.x), y = glm::radians(euler.y);
        return glm::vec3(cos(y) * cos(x), sin(y), cos(y) * sin(x));
    }
}