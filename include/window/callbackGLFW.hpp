#pragma once

#include "config.hpp"

#include "GLFW/glfw3.h"

/**
 * @brief GLFW callbacks.
 */
namespace callbackGLFW {
    void setWindowCallbacks(GLFWwindow* window);

    /**
     *  @brief Logs GLFW errors.
     * 
     * @param description error description, c-string
     */
    void error(int error, const char* description);

    /**
     * @brief Called by GLFW on window resize event. Re-sets GL viewport size.
     * GLAD must be initialized since GL calls are made to set the viewport.
     * @param window window of event
     */
    void windowResize(GLFWwindow* window, int width, int height);

    void keyAction(GLFWwindow* window, int key, int scancode, int action, int mods);

    void cursor(GLFWwindow* window, double xPos, double yPos);

    void scroll(GLFWwindow* window, double xOffset, double yOffset);
}