#pragma once

#include "config.hpp"

#include "GLFW/glfw3.h"

namespace input {
    struct references {
        int* mouseEnabled;
        double* mouseSensitivity;

        float* cameraX;
        float* cameraY;
    };

    void registerInput(input::references* inputStruct);

    void processKey(GLFWwindow* window, int key, int scancode, int action, int mods);
    void processCursor(GLFWwindow* window, double xPos, double yPos);
    void processScroll(GLFWwindow* window, double xOffset, double yOffset);
}