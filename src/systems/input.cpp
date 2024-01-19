#include "input.hpp"

#include "callbackGLFW.hpp"

namespace input {
    static input::references* data;

    void registerInput(input::references* inputStruct) {
        input::data = inputStruct;
    }

    void processKey(GLFWwindow* window, int key, int scancode, int action, int mods) {
        switch (key) {
            case GLFW_KEY_ESCAPE:
                if (action == GLFW_PRESS)
                    glfwSetWindowShouldClose(window, 1);
                break;
            case GLFW_KEY_SPACE:
                if (action == GLFW_PRESS)
                    if(glfwGetInputMode(window, GLFW_CURSOR) == GLFW_CURSOR_NORMAL) {
                        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
                        *data->mouseEnabled = 0;
                    }
                    else {
                        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
                        *data->mouseEnabled = 1;
                    }
                break;
            default:
                0;
        }

        if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
            glfwSetWindowShouldClose(window, 1);
        }
    }

    void processCursor(GLFWwindow* window, double xPos, double yPos) {
        static double xPosPrev = 0.0;
        static double yPosPrev = 0.0;

        double deltaX = xPos - xPosPrev;
        double deltaY = yPos - yPosPrev;

        xPosPrev = xPos;
        yPosPrev = yPos;

        if(!(*data->mouseEnabled)) {
            *data->cameraY -= *(data->mouseSensitivity) * deltaX;
            *data->cameraX -= *(data->mouseSensitivity) * deltaY;
        }
    }

    void processScroll(GLFWwindow* window, double xOffset, double yOffset) {

    }
}