#include "main.hpp"

#include "lifetime.hpp"
#include "callbackGLFW.hpp"
#include "shader.hpp"

#include "GLFW/glfw3.h"
#include "glad/glad.h"
#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include "spdlog/spdlog.h"
#include "stb_image.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include <stdlib.h>
#include <time.h>

GLFWwindow *createWindow(int windowWidth, int windowHeight, const char *windowTitle) {
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    GLFWwindow *window = glfwCreateWindow(windowWidth, windowHeight, windowTitle, NULL, NULL);
    if (window == NULL)
    {
        spdlog::error("GLFW couldn't create window.");
        lifetime::killAll(1);
    }
    return window;
}

glm::vec3 cameraPosition = glm::vec3(0.f, 0.f, 1.f);

namespace common {
    int16_t framebufferWidth = cfg::window::width;
    int16_t framebufferHeight = cfg::window::height;
    float fov = cfg::window::fov;
}

/**
 * @brief IMGUI global vars.
 */
namespace ImVars
{
    static GLfloat position[3] = {0.f, 0.f, 0.f};
    static GLfloat rotation[3] = { -45.f, -45.f, -45.f};
    static GLfloat scale [3]= { 1.f, 1.f, 1.f};
    static ImVec4 framebufferClearColor = ImVec4(0.92f, 0.92f, 0.92f, 1.0f);
    static ImVec4 vertexColors[4] = {
        ImVec4(0.5f, 0.2f, 1.0f, 1.0f),
        ImVec4(0.5f, 1.0f, 0.2f, 1.0f),
        ImVec4(1.0f, 0.5f, 0.2f, 1.0f),
        ImVec4(1.0f, 0.2f, 0.5f, 1.0f)};
    static GLfloat mixWeight = 0.6f;
    static bool isWireframe = 0;
    float& fov = common::fov; 
}

/**
 * @brief OpenGL shader uniform locations.
 */
namespace GLUniformPositions
{
    static GLint vertexColors = 0;
    static GLint texture0Sampler = 0;
    static GLint texture1Sampler = 0;
    static GLint mixWeight = 0;
    static GLint modelMatrix = 0;
    static GLint viewMatrix = 0;
    static GLint projectionMatrix = 0;
}

glm::mat4 modelMatrix = glm::mat4(1.f);
glm::mat4 viewMatrix = glm::translate(glm::mat4(1.f), -cameraPosition);
glm::mat4 projectionMatrix = glm::perspective(glm::radians(ImVars::fov), (float)cfg::window::width / cfg::window::height, 0.1f, 100.0f);


int main(int argc, char **args) {
    /**
     * INIT SPDLOG
     */
    lifetime::initSPDLOG();
    spdlog::info("Hello world, from spdlog.");

    /**
     * SETUP STBI
     */
    stbi_set_flip_vertically_on_load(1);

    /**
     * INIT GLFW, SET ERROR CALLBACK
     */
    lifetime::initGLFW();
    spdlog::info("GLFW init.");
    glfwSetErrorCallback((GLFWerrorfun)&callbackGLFW::error);

    /**
     * CREATE CONTEXT, SET CURRENT, SET STUFF
     */
    GLFWwindow *window = createWindow(cfg::window::width, cfg::window::height, cfg::window::title);
    glfwSetWindowPos(window, 100, 100);

    glfwSetWindowSizeLimits(window, cfg::window::width, cfg::window::height, GLFW_DONT_CARE, GLFW_DONT_CARE);

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    /**
     * LOAD GL, FRAMEBUFFER SETUP
     */
    bool gladStatus = gladLoadGLLoader((GLADloadproc)&glfwGetProcAddress);
    if (!gladStatus) {
        spdlog::error("GLAD couldn't load GLProc's");
        lifetime::killAll(1);
    }
    callbackGLFW::windowResize(window, 0, 0); // Call callback to set framebuffer size. Very smort...

    /**
     * SET CALLBACKS FOR WINDOW
     */
    glfwSetFramebufferSizeCallback(window, (GLFWframebuffersizefun)&callbackGLFW::windowResize);
    glfwSetKeyCallback(window, (GLFWkeyfun)&callbackGLFW::keyAction);

    /**
     * SETUP IMGUI
     */
    lifetime::initIMGUI(window);
    ImGui::StyleColorsLight();
    srand(time(NULL));
    ImVars::position[0] = 2.f - ((rand() % 401) / 100.f);
    ImVars::position[1] = 2.f - ((rand() % 401) / 100.f);
    ImVars::position[2] = -3.f - ((rand() % 401) / 100.f);
    ImVars::rotation[0] = 180.f - ((rand() % 3601) / 10.f);
    ImVars::rotation[1] = 180.f - ((rand() % 3601) / 10.f);
    ImVars::rotation[2] = 180.f - ((rand() % 3601) / 10.f);
    ImVars::scale[0] = 4.f - ((rand() % 201) / 100.f);
    ImVars::scale[1] = 4.f - ((rand() % 201) / 100.f);
    ImVars::scale[2] = 4.f - ((rand() % 201) / 100.f);

    /**
     * SETUP VBO & EBO
     */
    const GLfloat vertices[] = {
        //VERTEX 3x(float), TEXTURE COORD. 2x(float)
         0.5f,  0.5f,  0.0f,  1.f, 1.f,
         0.5f, -0.5f,  0.0f,  1.f, 0.f,
        -0.5f, -0.5f,  0.0f,  0.f, 0.f,
        -0.5f,  0.5f,  0.0f,  0.f, 1.f
    };

    const GLuint indices[] = {
        0, 1, 3,
        1, 2, 3
    };

    GLuint VBO = 0;
    {
        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);

        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    }

    GLuint EBO = 0;
    {
        glGenBuffers(1, &EBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    }

    /**
     * SETUP VAO FOR OBJECT
     */
    GLuint VAO = 0;
    {
        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(3*sizeof(float)));
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

        glBindVertexArray(0);
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

        int textureX, textureY, textureChannels;
        unsigned char* textureData = stbi_load(PROJECT_PATH "res/texture/kenney/png/Dark/texture_13.png", &textureX, &textureY, &textureChannels, 3);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, textureX, textureY, 0, GL_RGB, GL_UNSIGNED_BYTE, textureData);
        glGenerateMipmap(GL_TEXTURE_2D);

        stbi_image_free(textureData);
    }

    /**
     * SETUP THE SHADER, GET UNIFORM LOCATIONS
     */
    GLuint vertexShader = shader::compileShaderFromPath(PROJECT_PATH "res/shader/vertex.glsl", GL_VERTEX_SHADER);
    GLuint fragmentShader = shader::compileShaderFromPath(PROJECT_PATH "res/shader/fragment.glsl", GL_FRAGMENT_SHADER);
    GLuint shaderProgram = glCreateProgram();
    {
        glAttachShader(shaderProgram, vertexShader);
        glAttachShader(shaderProgram, fragmentShader);
        glLinkProgram(shaderProgram);

        GLUniformPositions::vertexColors = glGetUniformLocation(shaderProgram, "vertexColors");
        GLUniformPositions::texture0Sampler = glGetUniformLocation(shaderProgram, "texture0");
        GLUniformPositions::texture1Sampler = glGetUniformLocation(shaderProgram, "texture1");
        GLUniformPositions::mixWeight = glGetUniformLocation(shaderProgram, "mixWeight");

        GLUniformPositions::modelMatrix = glGetUniformLocation(shaderProgram, "modelMatrix");
        GLUniformPositions::viewMatrix = glGetUniformLocation(shaderProgram, "viewMatrix");
        GLUniformPositions::projectionMatrix = glGetUniformLocation(shaderProgram, "projectionMatrix");

        glUseProgram(shaderProgram);
        glUniform1i(GLUniformPositions::texture0Sampler, 0);
        glUniform1i(GLUniformPositions::texture1Sampler, 1);
    }

    while (1) {
        /**
         * CLEAR
         */
        glClearColor(ImVars::framebufferClearColor.x, ImVars::framebufferClearColor.y,
                        ImVars::framebufferClearColor.z, ImVars::framebufferClearColor.w);
        glClear(GL_COLOR_BUFFER_BIT);

        /**
         * OPENGL RENDER
         */
        if(ImVars::isWireframe) {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        }
        else {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        }

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);

        glUseProgram(shaderProgram);

        GLfloat copyBuffer[4][4];
        for (size_t i = 0; i < 4; i++) {
            copyBuffer[i][0] = ImVars::vertexColors[i].x;
            copyBuffer[i][1] = ImVars::vertexColors[i].y;
            copyBuffer[i][2] = ImVars::vertexColors[i].z;
            copyBuffer[i][3] = ImVars::vertexColors[i].w;
        }
        glUniform4fv(GLUniformPositions::vertexColors, 4, (GLfloat *)copyBuffer);

        glUniform1f(GLUniformPositions::mixWeight, ImVars::mixWeight);

        modelMatrix = glm::translate(glm::mat4(1.f), glm::vec3(ImVars::position[0], ImVars::position[1], ImVars::position[2]));
        modelMatrix = glm::rotate(modelMatrix, glm::radians((float)ImVars::rotation[0]), glm::vec3(1.f, 0.f, 0.f));
        modelMatrix = glm::rotate(modelMatrix, glm::radians((float)ImVars::rotation[1]), glm::vec3(0.f, 1.f, 0.f));
        modelMatrix = glm::rotate(modelMatrix, glm::radians((float)ImVars::rotation[2]), glm::vec3(0.f, 0.f, 1.f));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(ImVars::scale[0], ImVars::scale[1], ImVars::scale[2]));
        glUniformMatrix4fv(GLUniformPositions::modelMatrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));
        glm::translate(glm::mat4(1.f), -cameraPosition);
        glUniformMatrix4fv(GLUniformPositions::viewMatrix, 1, GL_FALSE, glm::value_ptr(viewMatrix));
        projectionMatrix = glm::perspective(glm::radians(common::fov), (float)common::framebufferWidth / common::framebufferHeight, 0.1f, 100.0f);
        glUniformMatrix4fv(GLUniformPositions::projectionMatrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

        glBindVertexArray(VAO);

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)0);

        /**
         * IMGUI RENDER
         */
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        {
            ImGui::Begin("rov-sim-env GUI", NULL, ImGuiWindowFlags_AlwaysAutoResize);

            ImGui::SeparatorText("CAMERA");
            ImGui::SliderFloat("FOV", &ImVars::fov, 30.f, 90.f);

            ImGui::SeparatorText("TRANSFORM");
            ImGui::SliderFloat3("Translate", ImVars::position, -10.f, 10.f);
            ImGui::SliderFloat3("Scale", ImVars::scale, 0.01f, 10.f);
            ImGui::SliderFloat3("Rotate", ImVars::rotation, 180.f, -180.f);

            ImGui::SeparatorText("COLOR");
            ImGui::ColorEdit3("Clear", (float *)&ImVars::framebufferClearColor);
            for (size_t i = 0; i < 4; i++) {
                char colorSliderText[50];
                sprintf(colorSliderText, "Vertex %d", (int)i);
                ImGui::ColorEdit3(colorSliderText, (float *)&ImVars::vertexColors[i]);
            }
            ImGui::SliderFloat("Mix Weight", &ImVars::mixWeight, 0.f, 1.f);


            ImGui::SeparatorText("DEBUG");
            ImGui::Checkbox("Wireframe Toggle", &ImVars::isWireframe);

            ImGui::SeparatorText("PERFORMANCE");
            ImGui::Text("Application average %.3f ms/f rame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
            ImGui::End();
        }
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        /**
         * SWAP BUFFERS, POLL EVENTS
         */
        glfwSwapBuffers(window);
        glfwPollEvents();

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
