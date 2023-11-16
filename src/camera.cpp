#include "camera.hpp"
#include "window.hpp"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

namespace camera {
    glm::vec3 position = glm::vec3(0.f, 0.f, 1.f);
    float fov = 75.f;

    glm::mat4 viewMatrix;
    glm::mat4 projectionMatrix;

    void rebuildViewMatrix() {
        viewMatrix = glm::translate(glm::mat4(1.f), -position);
    }
    void rebuildProjectionMatrix() {
        projectionMatrix = glm::perspective(glm::radians(fov), (float)window::framebufferWidth / window::framebufferHeight, 0.1f, 100.0f);
    }
}