#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"
#include "glad/glad.h"
#include "spdlog/spdlog.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <iostream>

int main(int argc, char** args) {
    spdlog::set_level(spdlog::level::info);
    spdlog::info("Hello world, from spdlog.");
    
    if(glfwInit() != GLFW_TRUE) {
        spdlog::error("GLFW couldn't init.");
    }

    glfwTerminate();
}