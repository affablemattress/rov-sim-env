#include "camera.hpp"
#include "window.hpp"
#include "gui.hpp"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

namespace camera {
    glm::mat4 buildViewMatrix(const camera::object& camera) {
        glm::mat4 returnMatrix = glm::rotate(glm::mat4(1.f), glm::radians(-camera.rotation.x), { 1, 0, 0 } );
        returnMatrix = glm::rotate(returnMatrix, glm::radians(-camera.rotation.y), { 0, 1, 0 } );
        returnMatrix = glm::translate(returnMatrix, -camera.position);
        return returnMatrix;
    }
    glm::mat4 buildProjectionMatrix(const camera::object& camera) {
        return glm::perspective(glm::radians(camera.fov), (float) camera.framebufferWidth / camera.framebufferHeight, 0.1f, 100.0f);
    }
}