#include "callbackGLFW.hpp"

#include "lifetime.hpp"
#include "window.hpp"
#include "camera.hpp"
#include "input.hpp"

#include "glad/glad.h"
#include "spdlog/spdlog.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

namespace callbackGLFW {
    void setWindowCallbacks(GLFWwindow* window) {
        glfwSetFramebufferSizeCallback(window, (GLFWframebuffersizefun)&callbackGLFW::windowResize);
        glfwSetKeyCallback(window, (GLFWkeyfun)&callbackGLFW::keyAction);
        glfwSetCursorPosCallback(window, (GLFWcursorposfun)&callbackGLFW::cursor);
        glfwSetScrollCallback(window, (GLFWscrollfun)&callbackGLFW::scroll);
    }
    
    void error(int error, const char* description) {
        spdlog::error("GLFW error.\n   >GLFW description start\n\n{0}\n\n   >GLFW description end", description);
        lifetime::killAll(1);
    }

    void windowResize(GLFWwindow* window, int width, int height) {
        static int framebufferWidth, framebufferHeight;
        glfwGetFramebufferSize(window, &framebufferWidth, &framebufferHeight);
        glViewport(0, 0, framebufferWidth, framebufferHeight);

        window::framebufferWidth = framebufferWidth;
        window::framebufferHeight = framebufferHeight;
    }

    void keyAction(GLFWwindow* window, int key, int scancode, int action, int mods) {
        input::processKey(window, key, scancode, action, mods);
    }

    void cursor(GLFWwindow* window, double xPos, double yPos) {
        input::processCursor(window, xPos, yPos);
    }

    void scroll(GLFWwindow* window, double xOffset, double yOffset) {
        input::processScroll(window, xOffset, yOffset);
    }
}

