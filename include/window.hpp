#pragma once

#include "config.hpp"

#include "GLFW/glfw3.h"
#include "glm/glm.hpp"

#include <string>

namespace window {
    GLFWwindow *createWindow(int windowWidth, int windowHeight, std::string_view windowTitle);
    void configureWindowAndSetContext(GLFWwindow* window, int16_t minimumWidth, int16_t minimumHeight);
}