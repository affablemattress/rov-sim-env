#include "window.hpp"

#include "app.hpp"

#include "spdlog/spdlog.h"
#include "GLFW/glfw3.h"

namespace window {
    GLFWwindow* createWindow(int windowWidth, int windowHeight, std::string_view windowTitle) {
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        GLFWwindow* window = glfwCreateWindow(windowWidth, windowHeight, windowTitle.data(), NULL, NULL);
        if (window == NULL) {
            spdlog::error("GLFW couldn't create window.");
            app::lifetime::killAll(1);
        }
        return window;
    }

    void configureWindowAndSetContext(GLFWwindow* window, int16_t minimumWidth, int16_t minimumHeight) {
        glfwSetWindowPos(window, 100, 100);
        glfwSetWindowSizeLimits(window, minimumWidth, minimumHeight, GLFW_DONT_CARE, GLFW_DONT_CARE);
        glfwMakeContextCurrent(window);
        glfwSwapInterval(1);
    }
}

