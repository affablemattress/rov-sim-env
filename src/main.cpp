#include "config.hpp"

#include "lifetime.hpp"
#include "callbackGLFW.hpp"

#include "GLFW/glfw3.h"
#include "glad/glad.h"
#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include "spdlog/spdlog.h"

#include <stdlib.h>

/**
 * @brief IMGUI global vars.
 */
namespace IMVars {
    static GLfloat position[] = { 0.f, 0.f }; 
    static ImVec4 framebufferClearColor = ImVec4(0.92f, 0.92f, 0.92f, 1.0f);
    static ImVec4 vertexColors[4] = {
        ImVec4(0.5f, 0.2f, 1.0f, 1.0f),
        ImVec4(0.5f, 1.0f, 0.2f, 1.0f),
        ImVec4(1.0f, 0.5f, 0.2f, 1.0f),
        ImVec4(1.0f, 0.2f, 0.5f, 1.0f)
    };
}

namespace GLUniformPositions {
    static GLint position = 0;
    static GLint vertexColors = 0;
}

GLFWwindow* createWindow(int windowWidth, int windowHeight, const char* windowTitle) {
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    GLFWwindow* window = glfwCreateWindow(windowWidth, windowHeight, windowTitle, NULL, NULL);
    if (window == NULL) {
        spdlog::error("GLFW couldn't create window.");
        lifetime::killAll(1);
    }
    return window;
}

int main(int argc, char** args) {
///INIT SPDLOG
    lifetime::initSPDLOG();
    spdlog::info("Hello world, from spdlog.");


///INIT GLFW, SET ERROR CALLBACK
    lifetime::initGLFW();
    spdlog::info("GLFW init.");
    glfwSetErrorCallback((GLFWerrorfun)&callbackGLFW::error);


///CREATE CONTEXT, SET CURRENT, SET STUFF
    GLFWwindow* window = createWindow(cfg::window::width, cfg::window::height, cfg::window::title);
    glfwSetWindowPos(window, 100, 100);

    glfwSetWindowSizeLimits(window, cfg::window::width, cfg::window::height, GLFW_DONT_CARE, GLFW_DONT_CARE);

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);


///LOAD GL, FRAMEBUFFER SETUP
    if(!gladLoadGLLoader((GLADloadproc)&glfwGetProcAddress)) {
        spdlog::error("GLAD couldn't load GLProc's");
        lifetime::killAll(1);
    }
    callbackGLFW::windowResize(window, 0, 0); //Call callback to set framebuffer size. Very smort...


///SET CALLBACKS FOR WINDOW
    glfwSetFramebufferSizeCallback(window, (GLFWframebuffersizefun)&callbackGLFW::windowResize);
    glfwSetKeyCallback(window, (GLFWkeyfun)&callbackGLFW::keyAction);


///SETUP IMGUI (AND ITS BACKENDS)
    lifetime::initIMGUI(window);
    ImGui::StyleColorsLight();


///SETUP VBO & EBO
    const GLfloat vertices[] = {
        0.5f,  0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        -0.5f, -0.5f, 0.0f,
        -0.5f,  0.5f, 0.0f 
    };

    const GLuint indices[] = {
        0, 1, 3,
        1, 2, 3 
    };

    GLuint VBO = 0; 
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);


    GLuint EBO = 0;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

///SETUP VAO FOR OBJECT
    GLuint VAO = 0;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0); 

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

    glBindVertexArray(0);

///SETUP THE SHADER
    const GLchar* vertexShaderSource = 
        "#version 330 core\n"
        "layout (location = 0) in vec3 aPos;\n"
        "out vec4 vertexColor;\n"
        "uniform vec2 position;\n"
        "uniform vec4 vertexColors[4];\n"
        "void main() {\n"
        "    vertexColor = vertexColors[gl_VertexID % 4];\n"
        "    gl_Position = vec4(aPos.x + position.x, aPos.y + position.y, aPos.z, 1.0);\n"
        "}\0";

    const GLchar* fragmentShaderSource = 
        "#version 330 core\n"
        "in vec4 vertexColor;"
        "out vec4 fragColor;\n"
        "void main() {\n"
        "    fragColor = vertexColor;\n"
        "}\0";

    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    
    GLint  compilationStatus;
    GLchar compilationInfoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &compilationStatus);
    if(!compilationStatus) {
        glGetShaderInfoLog(vertexShader, 512, NULL, compilationInfoLog);
        spdlog::error("GLSL compilation error.\n   >GLSL error description start\n\n{0}\n   >GLSL error description end", compilationInfoLog);
        lifetime::killAll(1);
    }
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &compilationStatus);
    if(!compilationStatus) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, compilationInfoLog);
        spdlog::error("GLSL compilation error.\n   >GLSL error description start\n\n{0}\n   >GLSL error description end", compilationInfoLog);
        lifetime::killAll(1);
    }

    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

///GAME LOOP
    while(1) {
        ///OPENGL RENDER
        glClearColor(IMVars::framebufferClearColor.x, IMVars::framebufferClearColor.y, 
                     IMVars::framebufferClearColor.z, IMVars::framebufferClearColor.w);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);
        GLUniformPositions::position = glGetUniformLocation(shaderProgram, "position");
        GLUniformPositions::vertexColors = glGetUniformLocation(shaderProgram, "vertexColors");

        glUniform2fv(GLUniformPositions::position, 1, IMVars::position);
        GLfloat copyBuffer[4][4];
        for (size_t i = 0; i < 4; i++) {
            copyBuffer[i][0] = IMVars::vertexColors[i].x; copyBuffer[i][1] = IMVars::vertexColors[i].y; 
            copyBuffer[i][2] = IMVars::vertexColors[i].z; copyBuffer[i][3] = IMVars::vertexColors[i].w;
        }
        glUniform4fv(GLUniformPositions::vertexColors, 4, (GLfloat*)copyBuffer);

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)0);

        ///IMGUI RENDER
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::Begin("rov-sim-env GUI", NULL, ImGuiWindowFlags_AlwaysAutoResize);

        ImGui::SeparatorText("POSITION");
        ImGui::SliderFloat("X", &IMVars::position[0], -0.7f, 0.7f);
        ImGui::SliderFloat("Y", &IMVars::position[1], -0.7f, 0.7f);
        ImGui::SeparatorText("COLOR");
        ImGui::ColorEdit3("Clear", (float*)&IMVars::framebufferClearColor);
        for (size_t i = 0; i < 4; i++) {
            char colorSliderText[50];
            sprintf(colorSliderText, "Vertex %d", (int)i);
            ImGui::ColorEdit3(colorSliderText, (float*)&IMVars::vertexColors[i]);
        }
        ImGui::SeparatorText("PERFORMANCE");
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        ImGui::End();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());


        ///SWAP BUFFERS, POLL EVENTS
        glfwSwapBuffers(window);
        glfwPollEvents();

        ///EXIT?
        if(glfwWindowShouldClose(window)) {
            break;
        }
    }


///DESTROY IMGUI, (WINDOW & CONTEXT &) GLFW. THEN TERMINATE
    lifetime::killAll(0);
    exit(EXIT_SUCCESS);
}
