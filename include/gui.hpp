#pragma once

#include "config.hpp"

#include "object.hpp"

#include "glad/glad.h"
#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include <vector>


namespace gui {
    struct vars{
        float (*framebufferClearColor)[4];
        float* fov; 

        object::mainCube* mainCube;

        std::vector<object::batchCube>* batchCubes;

        bool isWireframe;
    };

    void init(gui::vars& vars);
    void render(gui::vars& vars);
}