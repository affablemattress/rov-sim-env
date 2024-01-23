#include "app.hpp"
#include "callbackGLFW.hpp"
#include "shader.hpp"
#include "window.hpp"
#include "camera.hpp"
#include "gui.hpp"
#include "math.hpp"
#include "object.hpp"
#include "input.hpp"

#include "GLFW/glfw3.h"
#include "glad/glad.h"
#include "spdlog/spdlog.h"
#include "stb_image.h"
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"

#include <stdlib.h>
#include <time.h>

int main(int argc, char **args) {
    app::lifetime::startApp();

    app::lifetime::initSPDLOG(spdlog::level::level_enum::info, "Started rov-sim-env...");
    app::lifetime::initSTBI();
    app::lifetime::initGLFW(callbackGLFW::error, "GLFW init.");

    GLFWwindow* window = window::createWindow(app::settings.defaultWindowWidth, app::settings.defaultWindowHeight, "rov-sim-env");
    window::configureWindowAndSetContext(window, app::settings.defaultWindowWidth, app::settings.defaultWindowHeight);
    callbackGLFW::setWindowCallbacks(window);

    app::lifetime::initGLAD();
    app::lifetime::initIMGUI(window);

    /**
     * INIT SCENE
     */
    callbackGLFW::windowResize(window, 0, 0); // Call resize callback to set window vars

    GLfloat pointLightPosition[3] = { 10.f, 10.f, 10.f };
    GLfloat pointLightColor[4] = { 1.f, 0.16f, 0.f };

    GLfloat ambientLightIntensity = 0.2f;
    GLfloat ambientLightColor[4] = { 0.f, 0.5f, 1.f };

    const GLfloat cubeVertices[] = {
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
    };

    const GLuint cubeIndices[] = {
        0,  1,  2,
        3,  4,  5,

        6,  7,  8,
        9, 10, 11,
        
        12, 13, 14,
        15, 16, 17,
        
        18, 19, 20,
        21, 22, 23,
        
        24, 25, 26,
        27, 28, 29,

        30, 31, 32,
        33, 34, 35
    };

    GLuint mainVBO = 0;
    glGenBuffers(1, &mainVBO);

    GLuint mainEBO = 0;
    glGenBuffers(1, &mainEBO);

    GLuint mainVAO = 0;
    glGenVertexArrays(1, &mainVAO);

    GLuint mainDiffuseMap = 0;
    glGenTextures(1, &mainDiffuseMap);

    shader::Program mainProgram;
    mainProgram.id = glCreateProgram();
    {
        {
            shader::compileProgram(mainProgram, PROJECT_PATH "res/shader/vertexMain.glsl", PROJECT_PATH "res/shader/fragmentMain.glsl");

            const GLchar* programUniforms[] = { 
                "modelMatrix", "viewMatrix", "projectionMatrix", 
                "normalMatrix", 
                "diffuseMap", 
                "ambientLightColor", "ambientLightIntensity", 
                "pointLightColor", "pointLightPos" };
            shader::pushUniforms(mainProgram, sizeof(programUniforms) / sizeof(GLchar*), programUniforms);

            glUseProgram(mainProgram.id);
            shader::setUniform(mainProgram, glUniform1i, "diffuseMap", 0);

            shader::setUniform(mainProgram, glUniform1f, "ambientLightIntensity", ambientLightIntensity);
            shader::setUniform(mainProgram, glUniform3fv, "ambientLightColor", 1, ambientLightColor);

            shader::setUniform(mainProgram, glUniform3fv, "pointLightPos", 1, pointLightPosition);
            shader::setUniform(mainProgram, glUniform3fv, "pointLightColor", 1, pointLightColor);
        }

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
            GLint textureX, textureY, textureChannels;
            stbi_uc* textureData = stbi_load(PROJECT_PATH "res/texture/kenney/png/Dark/texture_13.png", &textureX, &textureY, &textureChannels, 3);

            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, mainDiffuseMap);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, textureX, textureY, 0, GL_RGB, GL_UNSIGNED_BYTE, textureData);
            //stbi_image_free(textureData);
            glGenerateMipmap(GL_TEXTURE_2D);
        }
    }

    GLuint batchDiffuseMap = 0;
    glGenTextures(1, &batchDiffuseMap);

    shader::Program batchProgram;
    batchProgram.id = glCreateProgram();
    {
        {
            GLint textureX, textureY, textureChannels;
            stbi_uc* textureData = stbi_load(PROJECT_PATH "res/texture/concrete.png", &textureX, &textureY, &textureChannels, 3);

            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, batchDiffuseMap);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, textureX, textureY, 0, GL_RGB, GL_UNSIGNED_BYTE, textureData);
            //stbi_image_free(textureData);
            glGenerateMipmap(GL_TEXTURE_2D);
        }

        {
            shader::compileProgram(batchProgram, PROJECT_PATH "res/shader/vertexBatch.glsl", PROJECT_PATH "res/shader/fragmentBatch.glsl");

            const GLchar* programUniforms[6] = { "mixColor", "modelMatrix", "viewMatrix", "projectionMatrix", "mixWeight", "texture0"};
            shader::pushUniforms(batchProgram, 6, programUniforms);

            glUseProgram(batchProgram.id);
            shader::setUniform(batchProgram, glUniform1i, "texture0", 0);
        }
    }

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

    gui::References guiRefs = {
        .camera = &mainCamera,
        .mainCube = &mainCubeData,
        .batchCubes = &batchCubes,
    };
    gui::registerRefs(&guiRefs);

    input::References inputRefs = {
        .camera = &mainCamera,
    };
    input::registerRefs(&inputRefs);
    glfwSetInputMode(window, GLFW_STICKY_KEYS, 1);
    if (glfwRawMouseMotionSupported())
        glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);

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


            glm::mat4 viewMatrix = camera::buildViewMatrix(mainCamera);

            mainCamera.framebufferWidth = app::window_vars.framebufferWidth;
            mainCamera.framebufferHeight = app::window_vars.framebufferHeight;
            glm::mat4 projectionMatrix = camera::buildProjectionMatrix(mainCamera);

            {
                glUseProgram(mainProgram.id);
                shader::setUniform(mainProgram, glUniformMatrix4fv, "viewMatrix", 1, GL_FALSE, glm::value_ptr(viewMatrix));
                shader::setUniform(mainProgram, glUniformMatrix4fv, "projectionMatrix", 1, GL_FALSE, glm::value_ptr(projectionMatrix));

                glBindVertexArray(mainVAO);

                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, mainDiffuseMap);

                glm::mat4 modelMatrix = math::buildModelMatrix(mainCubeData.position, mainCubeData.rotation, mainCubeData.scale);
                shader::setUniform(mainProgram, glUniformMatrix4fv, "modelMatrix", 1, GL_FALSE, glm::value_ptr(modelMatrix));

                glm::mat3 normalMatrix = math::buildNormalMatrixFromModelMatrix(modelMatrix);
                shader::setUniform(mainProgram, glUniformMatrix3fv, "normalMatrix", 1, GL_FALSE, glm::value_ptr(normalMatrix));

                glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, (void *)0);
            }

            {
                glUseProgram(batchProgram.id);
                shader::setUniform(batchProgram, glUniformMatrix4fv, "viewMatrix", 1, GL_FALSE, glm::value_ptr(viewMatrix));
                shader::setUniform(batchProgram, glUniformMatrix4fv, "projectionMatrix", 1, GL_FALSE, glm::value_ptr(projectionMatrix));

                glBindVertexArray(mainVAO);

                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, batchDiffuseMap);

                shader::setUniform(batchProgram, glUniform1f, "mixWeight", 0.9f);
                for(size_t i = 0; i < batchCubes.size(); i++) {
                    glm::mat4 modelMatrix = math::buildModelMatrix(batchCubes.at(i).position, batchCubes.at(i).rotation, batchCubes.at(i).scale);
                    shader::setUniform(batchProgram, glUniformMatrix4fv, "modelMatrix", 1, GL_FALSE, glm::value_ptr(modelMatrix));
                    
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
