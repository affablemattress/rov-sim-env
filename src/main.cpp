#include "lifetime.hpp"
#include "callbackGLFW.hpp"
#include "shader.hpp"
#include "window.hpp"
#include "camera.hpp"
#include "gui.hpp"
#include "math.hpp"

#include "GLFW/glfw3.h"
#include "glad/glad.h"
#include "spdlog/spdlog.h"
#include "stb_image.h"
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"

#include <stdlib.h>
#include <time.h>

int main(int argc, char **args) {
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
    shader::program program;
    program.id = glCreateProgram();
    shader::compileProgram(&program, PROJECT_PATH "res/shader/vertexMain.glsl", PROJECT_PATH "res/shader/fragmentMain.glsl");

    const GLchar* programUniforms[7] = { "vertexColors", "modelMatrix", "viewMatrix", "projectionMatrix", "mixWeight", "texture0", "texture1"};
    shader::pushUniforms(&program, 7, programUniforms);

    glUseProgram(program.id);
    glUniform1i(program.uniforms["texture0"], 0);
    glUniform1i(program.uniforms["texture1"], 1);

    /**
     * PUSH DATA TO VBO & EBO
     */
    GLuint VBO = 0;
    glGenBuffers(1, &VBO);
    GLuint EBO = 0;
    glGenBuffers(1, &EBO);
    {
        //VBO
        const GLfloat vertices[] = {
            //VERTEX 3x(float), TEXTURE COORD. 2x(float)
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
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        //EBO
        const GLuint indices[] = {
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
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    }

    /**
     * SETUP VAO FOR OBJECT
     */
    GLuint VAO = 0;
    glGenVertexArrays(1, &VAO);
    {
        glBindVertexArray(VAO);

        //SETUP MODEL'S VAO
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(3*sizeof(float)));
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    }

    /**
     * SETUP TEXTURE
     */
    GLuint texture = 0;
    glGenTextures(1, &texture);
    {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        GLint textureX, textureY, textureChannels;
        stbi_uc* textureData = stbi_load(PROJECT_PATH "res/texture/kenney/png/Dark/texture_13.png", &textureX, &textureY, &textureChannels, 3);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, textureX, textureY, 0, GL_RGB, GL_UNSIGNED_BYTE, textureData);
        glGenerateMipmap(GL_TEXTURE_2D);

        stbi_image_free(textureData);
    }

    camera::object mainCamera = { 
        .framebufferClearColor = { 0.92f, 0.92f, 0.92f, 1.0f },
        .position = { 0.0f, 0.0f, 1.0f },
        .fov = 75,
        .framebufferWidth = window::framebufferWidth,
        .framebufferHeight = window::framebufferHeight
    };

    struct _mainData {
        float position[3];
        float rotation[3];
        float scale[3];
        float vertexColors[4][4];
        float mixWeight;
    } mainObjectData = {
        .position = {0.f, 0.f, 0.f},
        .rotation = { -45.f, -45.f, -45.f},
        .scale = { 1.f, 1.f, 1.f },
        .vertexColors = { { 0.5f, 0.2f, 1.0f, 1.0f },
                          { 0.5f, 1.0f, 0.2f, 1.0f },
                          { 1.0f, 0.5f, 0.2f, 1.0f },
                          { 1.0f, 0.2f, 0.5f, 1.0f } },
        .mixWeight = 0.6f
    };

    gui::vars guiVars = {
        .framebufferClearColor = &mainCamera.framebufferClearColor,
        .fov = &mainCamera.fov,

        .mainObjectPosition = &mainObjectData.position,
        .mainObjectRotation = &mainObjectData.rotation,
        .mainObjectScale = &mainObjectData.scale,
        .vertexColors = &mainObjectData.vertexColors,
        .mixWeight = &mainObjectData.mixWeight,

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
            
            {
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, texture);

                glUseProgram(program.id);
                {
                    glUniform4fv(program.uniforms["vertexColors"], 4, (GLfloat *)guiVars.vertexColors);
                    glUniform1f(program.uniforms["mixWeight"], mainObjectData.mixWeight);
                    glUniformMatrix4fv(program.uniforms["viewMatrix"], 1, GL_FALSE, glm::value_ptr(viewMatrix));
                    glUniformMatrix4fv(program.uniforms["projectionMatrix"], 1, GL_FALSE, glm::value_ptr(projectionMatrix));

                    glm::mat4 modelMatrix = math::buildModelMatrix(mainObjectData.position, 
                                                                   mainObjectData.rotation, 
                                                                   mainObjectData.scale);
                    glUniformMatrix4fv(program.uniforms["modelMatrix"], 1, GL_FALSE, glm::value_ptr(modelMatrix));
                }

                glBindVertexArray(VAO);

                glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, (void *)0);
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
