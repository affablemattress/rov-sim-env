#pragma once

#include "config.hpp"

#include "GLFW/glfw3.h"
#include "camera.hpp"

namespace input {
    struct References {
        camera::Object* camera;
    };

    void registerRefs(input::References* refs);

    void processKeysAsync(GLFWwindow* window);
    
    void processKey(GLFWwindow* window, int key, int scancode, int action, int mods);
    void processCursor(GLFWwindow* window, double xPos, double yPos);
    void processScroll(GLFWwindow* window, double xOffset, double yOffset);
}