#include "config.hpp"

#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"
#include "glad/glad.h"
#include "spdlog/spdlog.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <iostream>
#include <stdlib.h>

void glfwErrorCallback(int error, const char* description);
void glfwWindowResizeCallback(GLFWwindow* window, int width, int height);
void glfwKeyActionCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
void killAll(void);


int main(int argc, char** args) {
///INIT SPDLOG
    spdlog::set_level(spdlog::level::info);
    spdlog::info("Hello world, from spdlog.");


///INIT GLFW, SET ERROR CALLBACK
    if(glfwInit() != GLFW_TRUE) {
        spdlog::error("GLFW couldn't init.");
        abort();
    }
    spdlog::info("GLFW init.");
    glfwSetErrorCallback(*glfwErrorCallback);


///GET CONTEXT, SET CURRENT, SET STUFF
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    GLFWwindow* window = glfwCreateWindow(cfg::window::width, cfg::window::height, cfg::window::title, NULL, NULL);
    if (window == NULL) {
        spdlog::error("GLFW couldn't create window.");
        killAll();
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);


///LOAD GL, FRAMEBUFFER SETUP
    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        spdlog::error("GLAD couldn't load GLProc's");
        killAll();
    }
    static int framebufferInitialWidth, framebufferInitialHeight; 
    glfwGetFramebufferSize(window, &framebufferInitialWidth, &framebufferInitialHeight);
    glViewport(0, 0, framebufferInitialWidth - 100, framebufferInitialHeight - 100);
    glfwSetFramebufferSizeCallback(window, (GLFWframebuffersizefun)&glfwWindowResizeCallback);


///SET INPUT CALLBACKS, 
    glfwSetKeyCallback(window, (GLFWkeyfun)&glfwKeyActionCallback);


///GAME LOOP
    while(!glfwWindowShouldClose(window)) {
        glfwSwapBuffers(window);
        glfwPollEvents();
    }


///DESTROY CONTEXT, TERMINATE
    glfwDestroyWindow(window);
    spdlog::info("GLFW terminating. [glfwTime : {0:.3f}(s)]", glfwGetTime());
    glfwTerminate();
    exit(EXIT_SUCCESS);
}

void glfwErrorCallback(int error, const char* description) {
    spdlog::error("GLFW error.\n GLFW description start | {0} | GLFW description end", description);
    killAll();
}

void glfwWindowResizeCallback(GLFWwindow* window, int width, int height) {
    static int framebufferWidth, framebufferHeight;
    glfwGetFramebufferSize(window, &framebufferWidth, &framebufferHeight);
    if(framebufferWidth <= 100 || framebufferHeight <= 100 ) {
        spdlog::error("Framebuffer size fell below 100px.");
        killAll();
    }
    glViewport(0, 0, framebufferWidth - 100, framebufferHeight - 100);
}

void glfwKeyActionCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, 1);
    }
}

/// @brief Kills all. Logs an error, terminates GLFW, then aborts.
inline void killAll(void) {
    spdlog::error("You've done it again... Committing dead. [glfwTime : {0:.3f}(s)]", glfwGetTime());
    glfwTerminate();
    abort();
}