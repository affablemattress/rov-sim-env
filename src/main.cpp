#include "app.hpp"
#include "math.hpp"

#include "window/window.hpp"
#include "window/callbackGLFW.hpp"

#include "systems/gui.hpp"
#include "systems/input.hpp"

#include "gameobjects/camera.hpp"
#include "gameobjects/object.hpp"

#include "scene/cubeData.hpp"

#include "render/texture.hpp"
#include "render/shader.hpp"
#include "render/uniforms/viewModel.hpp"
#include "render/uniforms/light.hpp"

#include "GLFW/glfw3.h"
#include "glad/glad.h"
#include "spdlog/spdlog.h"
#include "stb_image.h"
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"

#include <stdlib.h>
#include <time.h>

int main(int argc, char **args) {
// ! ||--------------------------------------------------------------------------------||
// ! ||                                 INITIALIZE APP                                 ||
// ! ||--------------------------------------------------------------------------------||
    app::lifetime::startApp();

    app::lifetime::initSPDLOG(spdlog::level::level_enum::info, "Started rov-sim-env...");
    app::lifetime::initSTBI();
    app::lifetime::initGLFW(callbackGLFW::error, "GLFW init.");

    GLFWwindow* window = window::createWindow(app::settings.defaultWindowWidth, app::settings.defaultWindowHeight, "rov-sim-env");
    window::configureWindowAndSetContext(window, app::settings.defaultWindowWidth, app::settings.defaultWindowHeight);
    callbackGLFW::setWindowCallbacks(window);

    app::lifetime::initGLAD();
    app::lifetime::initIMGUI(window);

    callbackGLFW::windowResize(window, 0, 0); // Call resize callback to set window vars

// ! ||--------------------------------------------------------------------------------||
// ! ||                              INITIALIZE RENDERERER                             ||
// ! ||--------------------------------------------------------------------------------||
    GLuint mainVBO = 0;
    glGenBuffers(1, &mainVBO);
    glBindBuffer(GL_ARRAY_BUFFER, mainVBO);

    GLuint mainEBO = 0;
    glGenBuffers(1, &mainEBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mainEBO);

    GLuint mainVAO = 0;
    glGenVertexArrays(1, &mainVAO);
    glBindVertexArray(mainVAO);

    GLuint mainDiffuseMap = 0, mainSpecularMap = 0;
    glGenTextures(1, &mainDiffuseMap);
    glGenTextures(1, &mainSpecularMap);
    glBindTexture(GL_TEXTURE_2D, mainDiffuseMap);
    glBindTexture(GL_TEXTURE_2D, mainSpecularMap);

    GLuint u_cameraData = 0, u_modelData = 0, u_lightData =0;
    glGenBuffers(1, &u_cameraData);
    glGenBuffers(1, &u_modelData);
    glGenBuffers(1, &u_lightData);
    glBindBufferBase(GL_UNIFORM_BUFFER, 0, u_cameraData);
    glBindBufferBase(GL_UNIFORM_BUFFER, 1, u_modelData);
    glBindBufferBase(GL_UNIFORM_BUFFER, 2, u_lightData);


    shader::Program mainProgram;
    mainProgram.id = glCreateProgram();
    {
        {
            glBindBuffer(GL_ARRAY_BUFFER, mainVBO);
            glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);

            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mainEBO);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cubeIndices), cubeIndices, GL_STATIC_DRAW);
        }

        {
            glBindVertexArray(mainVAO);
            {
                glBindBuffer(GL_ARRAY_BUFFER, mainVBO);
                glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
                glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(3*sizeof(float)));
                glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(6*sizeof(float)));
                glEnableVertexAttribArray(0);
                glEnableVertexAttribArray(1);
                glEnableVertexAttribArray(2);
                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mainEBO);
            }
        }

        {
            texture::loadTexture2D(PROJECT_PATH "res/texture/learnopengl/container/diffuse.png", mainDiffuseMap);
            texture::loadTexture2D(PROJECT_PATH "res/texture/learnopengl/container/specular.png", mainSpecularMap);
        }

        {
            shader::compileProgram(mainProgram, PROJECT_PATH "res/shader/vertexMain.glsl", PROJECT_PATH "res/shader/fragmentMain.glsl");

            const GLchar* programUniforms[] = { 
                "diffuseMap", 
                "specularMap",
                "specularShininess"
            };
            shader::pushUniforms(mainProgram, sizeof(programUniforms) / sizeof(GLchar*), programUniforms);

            glUseProgram(mainProgram.id);
            shader::setUniform(mainProgram, glUniform1i, "diffuseMap", 0);
            shader::setUniform(mainProgram, glUniform1i, "specularMap", 1);

            shader::bindUniformBlock(mainProgram, "ModelData", 1);
            shader::bindUniformBlock(mainProgram, "LightData", 2);
        }
    }

    GLuint batchDiffuseMap = 0;
    glGenTextures(1, &batchDiffuseMap);

    shader::Program batchProgram;
    batchProgram.id = glCreateProgram();
    {
        {
            texture::loadTexture2D(PROJECT_PATH "res/texture/kenney/png/Dark/texture_13.png", batchDiffuseMap);
        }

        {
            shader::compileProgram(batchProgram, PROJECT_PATH "res/shader/vertexBatch.glsl", PROJECT_PATH "res/shader/fragmentBatch.glsl");

            glUseProgram(batchProgram.id);

            const GLchar* programUniforms[] = { "mixColor", "mixWeight", "diffuseMap"};
            shader::pushUniforms(batchProgram,  sizeof(programUniforms) / sizeof(GLchar*), programUniforms);

            shader::setUniform(batchProgram, glUniform1i, "diffuseMap", 0);

            shader::bindUniformBlock(batchProgram, "CameraData", 0);
            shader::bindUniformBlock(batchProgram, "ModelData", 1);
            shader::bindUniformBlock(batchProgram, "LightData", 2);
        }
    }

// ! ||--------------------------------------------------------------------------------||
// ! ||                                INITIALIZE SCENE                                ||
// ! ||--------------------------------------------------------------------------------||
    uniform::PointLight pointLight({ 0.f, 0.5f, 1.f, 1.f }, { 1.f, 0.9f, 0.9f, 1.f }, { -0.6f, 3.2f, -0.191f },
        0.2f, 0.5f, 1.f, 1.f, 1.f);
    uniform::PointLight pointLight2({ 0.8f, 0.2f, 0.f, 1.f }, { 1.f, 0.0f, 0.f, 1.f }, { -3.f, 4.f, -4.f },
        0.2f, 0.5f, 1.f, 1.f, 1.f);

    uniform::LightData lightData;
    lightData.pointLights[0] = pointLight;
    lightData.pointLights[1] = pointLight2;
    lightData.pointLightCount = 2;

    GLfloat specularShininess = 32.f;

    camera::Object mainCamera = { 
        .framebufferClearColor = { 0.92f, 0.92f, 0.92f, 1.0f },
        .position = { 2.f, 2.0f, 2.0f },
        .rotation = { 135.f, -45.f },
        .fov = 75,
        .framebufferWidth = app::window_vars.framebufferWidth,
        .framebufferHeight = app::window_vars.framebufferHeight
    };

    object::MainCube mainCubeData = {
        .position = {0.f, 0.f, 0.f},
        .rotation = { 30.f, 60.f, 0.f},
        .scale = { 1.f, 1.f, 1.f },
    };

    std::vector<object::BatchCube> batchCubes;

// ! ||--------------------------------------------------------------------------------||
// ! ||                              SET SYSTEM REFERENCES                             ||
// ! ||--------------------------------------------------------------------------------||
    gui::References guiRefs = {
        .camera = &mainCamera,
        .mainCube = &mainCubeData,
        .batchCubes = &batchCubes,

        .pointLightPosition = glm::value_ptr(pointLight.pos),
        .pointLightColor = glm::value_ptr(pointLight.color),

        .specularShininess = &specularShininess,
        .specularStrength = &pointLight.specularIntensity,

        .ambientLightIntensity = &pointLight.ambientIntensity,
        .ambientLightColor = glm::value_ptr(pointLight.ambientColor)
    };
    gui::registerRefs(&guiRefs);

    input::References inputRefs = {
        .camera = &mainCamera,
    };
    input::registerRefs(&inputRefs);
    glfwSetInputMode(window, GLFW_STICKY_KEYS, 1);
    if (glfwRawMouseMotionSupported())
        glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);

// ! ||--------------------------------------------------------------------------------||
// ! ||                                    GAME LOOP                                   ||
// ! ||--------------------------------------------------------------------------------||
    double prevFrameTime = 0.;
    double currentFrameTime = 0.;
    while (1) {
        currentFrameTime = glfwGetTime();
        app::window_vars.frametime = currentFrameTime - prevFrameTime;
        prevFrameTime = currentFrameTime;

        glfwPollEvents();
        input::processKeysAsync(window);

        /**
         * OPENGL RENDER
         */
        {
            glClearColor(mainCamera.framebufferClearColor[0], mainCamera.framebufferClearColor[1],
                         mainCamera.framebufferClearColor[2], mainCamera.framebufferClearColor[3]);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            glEnable(GL_DEPTH_TEST);

            if(app::state_vars.isWireframe) {
                glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            }
            else {
                glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            }

            mainCamera.framebufferWidth = app::window_vars.framebufferWidth;
            mainCamera.framebufferHeight = app::window_vars.framebufferHeight;
            uniform::CameraData cameraMatrices(mainCamera);
            glBindBuffer(GL_UNIFORM_BUFFER, u_cameraData);
            glBufferData(GL_UNIFORM_BUFFER, sizeof(uniform::CameraData), &cameraMatrices, GL_DYNAMIC_DRAW);

            {
                glUseProgram(mainProgram.id);

                glBindVertexArray(mainVAO);

                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, mainDiffuseMap);
                glActiveTexture(GL_TEXTURE0 + 1);
                glBindTexture(GL_TEXTURE_2D, mainSpecularMap);


                uniform::ModelData modelData(mainCubeData.position, mainCubeData.rotation, mainCubeData.scale);
                glBindBuffer(GL_UNIFORM_BUFFER, u_modelData);
                glBufferData(GL_UNIFORM_BUFFER, sizeof(uniform::ModelData), &modelData, GL_DYNAMIC_DRAW);

                lightData.pointLights[0] = pointLight;
                glBindBuffer(GL_UNIFORM_BUFFER, u_lightData);
                glBufferData(GL_UNIFORM_BUFFER, sizeof(uniform::LightData), &lightData, GL_DYNAMIC_DRAW);

                shader::setUniform(mainProgram, glUniform1f, "specularShininess", specularShininess);

                glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, (void *)0);
            }

            {
                glUseProgram(batchProgram.id);

                glBindVertexArray(mainVAO);

                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, batchDiffuseMap);

                shader::setUniform(batchProgram, glUniform1f, "mixWeight", 0.9f);

                for(size_t i = 0; i < batchCubes.size(); i++) {
                    uniform::ModelData modelData(batchCubes.at(i).position, batchCubes.at(i).rotation, batchCubes.at(i).scale);
                    glBindBuffer(GL_UNIFORM_BUFFER, u_modelData);
                    glBufferData(GL_UNIFORM_BUFFER, sizeof(uniform::ModelData), &modelData, GL_DYNAMIC_DRAW);
                    
                    shader::setUniform(batchProgram, glUniform4fv, "mixColor", 1, batchCubes.at(i).mixColor);

                    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, (void *)0);
                }
            }
        }

        gui::render();

        glfwSwapBuffers(window);

        if (glfwWindowShouldClose(window)) {
            break;
        }
    }

    app::lifetime::killAll(0);
    app::lifetime::endApp();
    exit(EXIT_SUCCESS);
}
