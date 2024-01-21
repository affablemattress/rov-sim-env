#include "input.hpp"

#include "callbackGLFW.hpp"
#include "math.hpp"
#include "app.hpp"

namespace input {
    static input::References* refs;

    void registerRefs(input::References* refs) {
        input::refs = refs;
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
                        app::state_vars.isMouseEnabled = 0;
                    }
                    else {
                        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
                        app::state_vars.isMouseEnabled = 1;
                    }
                break;
            case GLFW_KEY_W:
                if(!(app::state_vars.isMouseEnabled)) {
                    glm::vec3 direction = math::getDirectionVectorFrom2DEuler(refs->camera->rotation);
                    refs->camera->position += direction * (float)(app::settings.movementSpeed * app::window_vars.frametime);
                }

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

        if(!app::state_vars.isMouseEnabled) {
            refs->camera->rotation.x += app::settings.mouseSensitivity * deltaX;
            refs->camera->rotation.y -= app::settings.mouseSensitivity * deltaY;

            if (refs->camera->rotation.x > 180.0) refs->camera->rotation.x -= 360.0;
            if (refs->camera->rotation.x < -180.0) refs->camera->rotation.x += 360.0;
            if (refs->camera->rotation.y > 85.0) refs->camera->rotation.y = 85.0;
            if (refs->camera->rotation.y < -85.0) refs->camera->rotation.y = -85.0;
        }
    }

    void processScroll(GLFWwindow* window, double xOffset, double yOffset) {

    }
}