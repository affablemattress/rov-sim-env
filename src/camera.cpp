#include "camera.hpp"
#include "window.hpp"
#include "gui.hpp"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

namespace camera {
    glm::mat4 buildViewMatrix(const camera::object& camera) {
        return glm::translate(glm::mat4(1.f), -camera.position);
    }
    glm::mat4 buildProjectionMatrix(const camera::object& camera) {
        return glm::perspective(glm::radians(camera.fov), (float) camera.framebufferWidth / camera.framebufferHeight, 0.1f, 100.0f);
    }
}