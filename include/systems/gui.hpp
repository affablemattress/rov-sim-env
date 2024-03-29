#pragma once

#include "config.hpp"

#include "gameobjects/object.hpp"
#include "gameobjects/camera.hpp"

#include "glad/glad.h"
#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include <vector>


namespace gui {
    struct References{
        camera::Object* camera;

        object::MainCube* mainCube;

        std::vector<object::BatchCube>* batchCubes;

        GLfloat* pointLightPosition;
        GLfloat* pointLightColor;

        GLfloat* specularShininess;
        GLfloat* specularStrength;

        GLfloat* ambientLightIntensity;
        GLfloat* ambientLightColor;
    };

    extern void registerRefs(gui::References* refs);
    extern void render();
}