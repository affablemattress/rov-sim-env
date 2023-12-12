#pragma once

#include "config.hpp"

#include "glad/glad.h"
#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

namespace gui {
    struct vars{
        float (*framebufferClearColor)[4];
        float* fov; 
        
        float (*mainObjectPosition)[3];
        float (*mainObjectRotation)[3];
        float (*mainObjectScale)[3];
        float (*vertexColors)[4][4];
        GLfloat* mixWeight;

        bool isWireframe;
    };

    void init(gui::vars& vars);
    void render(gui::vars& vars);
}