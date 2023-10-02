#include "config.hpp"

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

#include <stdlib.h>
#include <time.h>

/**
 * @brief IMGUI global vars.
 */
namespace ImVars
{
    static GLfloat position[] = {0.f, 0.f};
    static ImVec4 framebufferClearColor = ImVec4(0.92f, 0.92f, 0.92f, 1.0f);
    static ImVec4 vertexColors[4] = {
        ImVec4(0.5f, 0.2f, 1.0f, 1.0f),
        ImVec4(0.5f, 1.0f, 0.2f, 1.0f),
        ImVec4(1.0f, 0.5f, 0.2f, 1.0f),
        ImVec4(1.0f, 0.2f, 0.5f, 1.0f)};
    static GLfloat mixWeight = 0;
    static bool isWireframe = 0;
}

/**
 * @brief OpenGL shader uniform locations.
 */
namespace GLUniformPositions
{
    static GLint position = 0;
    static GLint vertexColors = 0;
    static GLint texture0Sampler = 0;
    static GLint texture1Sampler = 0;
    static GLfloat mixWeight = 0;
}

GLFWwindow *createWindow(int windowWidth, int windowHeight, const char *windowTitle)
{
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

int main(int argc, char **args)
{
    /**
     * INIT SPDLOG
     */
    lifetime::initSPDLOG();
    spdlog::info("Hello world, from spdlog.");

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
    if (!gladStatus)
    {
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
    ImVars::position[1] = 0.7f - ((rand() % 140) / 100.f);
    ImVars::position[0] = 0.7f - ((rand() % 140) / 100.f);

    /**
     * SETUP VBO & EBO
     */
    const GLfloat vertices[] = {
        //VERTEX 3x(float), TEXTURE COORD. 2x(float)
         0.5f,  0.5f,  0.0f,  1.f, 1.f,
         0.5f, -0.5f,  0.0f,  1.f, 0.f,
        -0.5f, -0.5f,  0.0f,  0.f, 0.f,
        -0.5f,  0.5f,  0.0f,  0.f, 1.f};

    const GLuint indices[] = {
        0, 1, 3,
        1, 2, 3};

    GLuint VBO = 0;
    {
        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
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

    stbi_set_flip_vertically_on_load(1);
    GLuint texture = 0;
    glGenTextures(1, &texture);
    {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);

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

        GLUniformPositions::position = glGetUniformLocation(shaderProgram, "position");
        GLUniformPositions::vertexColors = glGetUniformLocation(shaderProgram, "vertexColors");
        GLUniformPositions::texture0Sampler = glGetUniformLocation(shaderProgram, "texture0");
        GLUniformPositions::texture1Sampler = glGetUniformLocation(shaderProgram, "texture1");
        GLUniformPositions::mixWeight = glGetUniformLocation(shaderProgram, "mixWeight");

        glUseProgram(shaderProgram);
        glUniform1i(GLUniformPositions::texture0Sampler, 0);
        glUniform1i(GLUniformPositions::texture1Sampler, 1);
    }

    while (1)
    {
        /**
         * CLEAR
         */
        {
            glClearColor(ImVars::framebufferClearColor.x, ImVars::framebufferClearColor.y,
                         ImVars::framebufferClearColor.z, ImVars::framebufferClearColor.w);
            glClear(GL_COLOR_BUFFER_BIT);
        }

        /**
         * OPENGL RENDER
         */
        {
            if(ImVars::isWireframe) {
                glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            }
            else {
                glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            }

            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, texture);

            glUseProgram(shaderProgram);
            glUniform2fv(GLUniformPositions::position, 1, ImVars::position);
            GLfloat copyBuffer[4][4];
            for (size_t i = 0; i < 4; i++)
            {
                copyBuffer[i][0] = ImVars::vertexColors[i].x;
                copyBuffer[i][1] = ImVars::vertexColors[i].y;
                copyBuffer[i][2] = ImVars::vertexColors[i].z;
                copyBuffer[i][3] = ImVars::vertexColors[i].w;
            }
            glUniform4fv(GLUniformPositions::vertexColors, 4, (GLfloat *)copyBuffer);
            glUniform1f(GLUniformPositions::mixWeight, ImVars::mixWeight);

            glBindVertexArray(VAO);

            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)0);
        }

        /**
         * IMGUI RENDER
         */
        {
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();
            {
                ImGui::Begin("rov-sim-env GUI", NULL, ImGuiWindowFlags_AlwaysAutoResize);

                ImGui::SeparatorText("POSITION");
                ImGui::SliderFloat("X", &ImVars::position[0], -0.7f, 0.7f);
                ImGui::SliderFloat("Y", &ImVars::position[1], -0.7f, 0.7f);

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
        }

        /**
         * SWAP BUFFERS, POLL EVENTS
         */
        glfwSwapBuffers(window);
        glfwPollEvents();

        /**
         * EXIT?
         */
        if (glfwWindowShouldClose(window))
        {
            break;
        }
    }

    /**
     * DESTROY IMGUI, (WINDOW & CONTEXT &) GLFW. THEN TERMINATE
     */
    lifetime::killAll(0);
    exit(EXIT_SUCCESS);
}
