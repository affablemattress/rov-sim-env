#include "callbackGLFW.hpp"

#include "main.hpp"
#include "lifetime.hpp"

#include "glad/glad.h"
#include "spdlog/spdlog.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

namespace callbackGLFW {
    void error(int error, const char* description) {
        spdlog::error("GLFW error.\n   >GLFW description start\n\n{0}\n\n   >GLFW description end", description);
        lifetime::killAll(1);
    }

    void windowResize(GLFWwindow* window, int width, int height) {
        static int framebufferWidth, framebufferHeight;
        glfwGetFramebufferSize(window, &framebufferWidth, &framebufferHeight);
        glViewport(0, 0, framebufferWidth, framebufferHeight);

        common::framebufferWidth = framebufferWidth;
        common::framebufferHeight = framebufferHeight;
        projectionMatrix = glm::perspective(glm::radians(common::fov), (float)common::framebufferWidth / common::framebufferHeight, 0.1f, 100.0f);
    }

    void keyAction(GLFWwindow* window, int key, int scancode, int action, int mods) {
        if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
            glfwSetWindowShouldClose(window, 1);
        }
    }
}

