#pragma once

#include "config.hpp"

#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"

namespace math {
    template <typename T>
    inline glm::mat4 buildModelMatrix(const T position[3], const T rotation[3], const T scale[3]) {
        glm::mat4 returnMatrix = glm::mat4(1.f);
        returnMatrix = glm::translate(returnMatrix, { position[0], position[1], position[2] });
        returnMatrix = glm::rotate(returnMatrix, glm::radians(rotation[0]), glm::vec3(1.f, 0.f, 0.f));
        returnMatrix = glm::rotate(returnMatrix, glm::radians(rotation[1]), glm::vec3(0.f, 1.f, 0.f));
        returnMatrix = glm::rotate(returnMatrix, glm::radians(rotation[2]), glm::vec3(0.f, 0.f, 1.f));
        returnMatrix = glm::scale(returnMatrix, { scale[0], scale[1], scale[2] });
        return returnMatrix;
    }
}