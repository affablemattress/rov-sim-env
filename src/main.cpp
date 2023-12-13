#include "lifetime.hpp"
#include "callbackGLFW.hpp"
#include "shader.hpp"
#include "window.hpp"
#include "camera.hpp"
#include "gui.hpp"
#include "math.hpp"
#include "object.hpp"

#include "GLFW/glfw3.h"
#include "glad/glad.h"
#include "spdlog/spdlog.h"
#include "stb_image.h"
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"

#include <stdlib.h>
#include <time.h>

int main(int argc, char **args) {
    srand(time(NULL));

    lifetime::initSPDLOG(spdlog::level::level_enum::info, "Started rov-sim-env...");
    lifetime::initSTBI();
    lifetime::initGLFW(callbackGLFW::error, "GLFW init.");
    glfwSetErrorCallback((GLFWerrorfun)&callbackGLFW::error);

    /**
     * CREATE WINDOW & CONTEXT, THEN CONFIGURE
     */
    GLFWwindow* window = window::createWindow(800, 600, "rov-sim-env");
    window::configureWindowAndSetContext(window, 800, 600);
    callbackGLFW::setWindowCallbacks(window);

    /**
     * LOAD GL, FRAMEBUFFER SETUP
     */
    lifetime::initGLAD();
    lifetime::initIMGUI(window);

    /**
     * INIT SCENE
     */
    callbackGLFW::windowResize(window, 0, 0); // Call resize callback to set window vars & projection matrix...

    /**
     * SETUP THE SHADER, GET UNIFORM LOCATIONS
     */    
    const GLfloat cubeVertices[] = {
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
        0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  0.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  1.0f, 0.0f,

        0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,

        0.5f, -0.5f,  0.5f,  1.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    };

    const GLuint cubeIndices[] = {
        0,  1,  2,
        2,  3,  0,

        4,  5,  6,
        6,  7,  4,
        
        8, 9, 10,
        10, 11, 8,
        
        12, 13, 14,
        14, 15, 12,
        
        0, 1, 16,
        16, 11, 0,

        17, 13, 6,
        6, 7, 17
    };

    GLuint mainVBO = 0;
    glGenBuffers(1, &mainVBO);

    GLuint mainEBO = 0;
    glGenBuffers(1, &mainEBO);

    GLuint mainVAO = 0;
    glGenVertexArrays(1, &mainVAO);

    GLuint mainTexture = 0;
    glGenTextures(1, &mainTexture);

    shader::program mainProgram;
    mainProgram.id = glCreateProgram();

    {
        shader::compileProgram(&mainProgram, PROJECT_PATH "res/shader/vertexMain.glsl", PROJECT_PATH "res/shader/fragmentMain.glsl");

        const GLchar* programUniforms[7] = { "vertexColors", "modelMatrix", "viewMatrix", "projectionMatrix", "mixWeight", "texture0", "texture1"};
        shader::pushUniforms(&mainProgram, 7, programUniforms);

        glUseProgram(mainProgram.id);
        glUniform1i(mainProgram.uniforms["texture0"], 0);
        glUniform1i(mainProgram.uniforms["texture1"], 1);
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
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
            glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(3*sizeof(float)));
            glEnableVertexAttribArray(0);
            glEnableVertexAttribArray(1);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mainEBO);
        }
    }

    {
        GLint textureX, textureY, textureChannels;
        stbi_uc* textureData = stbi_load(PROJECT_PATH "res/texture/kenney/png/Dark/texture_13.png", &textureX, &textureY, &textureChannels, 3);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, mainTexture);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, textureX, textureY, 0, GL_RGB, GL_UNSIGNED_BYTE, textureData);
        //stbi_image_free(textureData);
        glGenerateMipmap(GL_TEXTURE_2D);
    }

    GLuint batchTexture = 0;
    glGenTextures(1, &batchTexture);

    shader::program batchProgram;
    batchProgram.id = glCreateProgram();
    
    {
        GLint textureX, textureY, textureChannels;
        stbi_uc* textureData = stbi_load(PROJECT_PATH "res/texture/concrete.png", &textureX, &textureY, &textureChannels, 3);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, batchTexture);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, textureX, textureY, 0, GL_RGB, GL_UNSIGNED_BYTE, textureData);
        //stbi_image_free(textureData);
        glGenerateMipmap(GL_TEXTURE_2D);
    }

    {
        shader::compileProgram(&batchProgram, PROJECT_PATH "res/shader/vertexBatch.glsl", PROJECT_PATH "res/shader/fragmentBatch.glsl");

        const GLchar* programUniforms[6] = { "mixColor", "modelMatrix", "viewMatrix", "projectionMatrix", "mixWeight", "texture0"};
        shader::pushUniforms(&batchProgram, 6, programUniforms);

        glUseProgram(batchProgram.id);
        glUniform1i(batchProgram.uniforms["texture0"], 0);
    }

    camera::object mainCamera = { 
        .framebufferClearColor = { 0.92f, 0.92f, 0.92f, 1.0f },
        .position = { 0.0f, 0.0f, 1.0f },
        .fov = 75,
        .framebufferWidth = window::framebufferWidth,
        .framebufferHeight = window::framebufferHeight
    };

    object::mainCube mainCubeData = {
        .position = {0.f, 0.f, 0.f},
        .rotation = { -45.f, -45.f, -45.f},
        .scale = { 1.f, 1.f, 1.f },
        .vertexColors = { { 0.5f, 0.2f, 1.0f, 1.0f },
                          { 0.5f, 1.0f, 0.2f, 1.0f },
                          { 1.0f, 0.5f, 0.2f, 1.0f },
                          { 1.0f, 0.2f, 0.5f, 1.0f } },
        .mixWeight = 0.6f
    };

    std::vector<object::batchCube> batchCubes;

    gui::vars guiVars = {
        .framebufferClearColor = &mainCamera.framebufferClearColor,
        .fov = &mainCamera.fov,

        .mainCube = &mainCubeData,

        .batchCubes = &batchCubes,

        .isWireframe = 0 
    };

    gui::init(guiVars);

    while (1) {
        /**
         * OPENGL RENDER
         */
        {
            glClearColor(mainCamera.framebufferClearColor[0], mainCamera.framebufferClearColor[1],
                         mainCamera.framebufferClearColor[2], mainCamera.framebufferClearColor[3]);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            glEnable(GL_DEPTH_TEST);

            if(guiVars.isWireframe) {
                glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            }
            else {
                glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            }

            glm::mat4 viewMatrix = camera::buildViewMatrix(mainCamera);
            glm::mat4 projectionMatrix = camera::buildProjectionMatrix(mainCamera);

            glUseProgram(mainProgram.id);
            {
                glBindVertexArray(mainVAO);

                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, mainTexture);

                glm::mat4 modelMatrix = math::buildModelMatrix(mainCubeData.position, mainCubeData.rotation, mainCubeData.scale);

                glUniform4fv(mainProgram.uniforms["vertexColors"], 4, (GLfloat *)mainCubeData.vertexColors);
                glUniform1f(mainProgram.uniforms["mixWeight"], mainCubeData.mixWeight);
                glUniformMatrix4fv(mainProgram.uniforms["viewMatrix"], 1, GL_FALSE, glm::value_ptr(viewMatrix));
                glUniformMatrix4fv(mainProgram.uniforms["projectionMatrix"], 1, GL_FALSE, glm::value_ptr(projectionMatrix));
                glUniformMatrix4fv(mainProgram.uniforms["modelMatrix"], 1, GL_FALSE, glm::value_ptr(modelMatrix));

                glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, (void *)0);
            }

            glUseProgram(batchProgram.id);
            {
                glBindVertexArray(mainVAO);

                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, batchTexture);

                glUniformMatrix4fv(batchProgram.uniforms["viewMatrix"], 1, GL_FALSE, glm::value_ptr(viewMatrix));
                glUniformMatrix4fv(batchProgram.uniforms["projectionMatrix"], 1, GL_FALSE, glm::value_ptr(projectionMatrix));
                glUniform1f(batchProgram.uniforms["mixWeight"], 0.8f);
                for(size_t i = 0; i < batchCubes.size(); i++) {
                    glm::mat4 modelMatrix = math::buildModelMatrix(batchCubes.at(i).position, batchCubes.at(i).rotation, batchCubes.at(i).scale);
                    glUniformMatrix4fv(batchProgram.uniforms["modelMatrix"], 1, GL_FALSE, glm::value_ptr(modelMatrix));
                    glUniform4fv(batchProgram.uniforms["mixColor"], 1, batchCubes.at(i).mixColor);

                    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, (void *)0);
                }
            }
        }

        gui::render(guiVars);

        /**
         * SWAP BUFFERS, POLL EVENTS
         */
        {
            glfwSwapBuffers(window);
            glfwPollEvents();
        }

        /**
         * EXIT?
         */
        if (glfwWindowShouldClose(window)) {
            break;
        }
    }

    /**
     * DESTROY IMGUI, (WINDOW & CONTEXT &) GLFW. THEN TERMINATE
     */
    lifetime::killAll(0);
    exit(EXIT_SUCCESS);
}
