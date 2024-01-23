#include "input.hpp"

#include "callbackGLFW.hpp"
#include "math.hpp"
#include "app.hpp"

namespace input {
    static input::References* refs;

    void registerRefs(input::References* refs) {
        input::refs = refs;
    }

    void processKeysAsync(GLFWwindow* window) {
        if(!(app::state_vars.isMouseEnabled)) {
            if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
                glm::vec3 direction = math::getDirectionVectorFrom2DEuler(refs->camera->rotation);
                refs->camera->position += direction * (float)(app::settings.movementSpeed * app::window_vars.frametime);
            }
            if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
                glm::vec3 direction = math::getDirectionVectorFrom2DEuler(refs->camera->rotation);
                refs->camera->position -= direction * (float)(app::settings.movementSpeed * app::window_vars.frametime);
            }
            if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
                glm::vec3 direction = math::getDirectionVectorFrom2DEuler(refs->camera->rotation);
                refs->camera->position += glm::normalize(glm::cross(direction, { 0., 1., 0. })) * (float)(app::settings.movementSpeed * app::window_vars.frametime);
            }
            if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
                glm::vec3 direction = math::getDirectionVectorFrom2DEuler(refs->camera->rotation);
                refs->camera->position -= glm::normalize(glm::cross(direction, { 0., 1., 0. })) * (float)(app::settings.movementSpeed * app::window_vars.frametime);
            }
        }
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
            refs->camera->rotation.x -= app::settings.mouseSensitivity * deltaX;
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