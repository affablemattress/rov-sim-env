#include "config.hpp"

#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"
#include "glad/glad.h"
#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include "spdlog/spdlog.h"

#include <iostream>
#include <stdlib.h>

namespace lifetime {
    inline void initSPDLOG();
    inline void initGLFW();
    inline void initIMGUI(GLFWwindow* window);
    inline void killIMGUI();
    
    inline void killAll(uint8_t error);
}

namespace callbackGLFW {
    void error(int error, const char* description);
    void windowResize(GLFWwindow* window, int width, int height);
    void keyAction(GLFWwindow* window, int key, int scancode, int action, int mods);
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


///GAME LOOP
    while(1) {
        ///OPENGL RENDER
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);


        ///IMGUI RENDER
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::ShowDemoWindow(); 

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

namespace callbackGLFW {
    void error(int error, const char* description) {
        spdlog::error("GLFW error.\n   >GLFW description start\n\n{0}\n\n   >GLFW description end", description);
        lifetime::killAll(1);
    }

    void windowResize(GLFWwindow* window, int width, int height) {
        static int framebufferWidth, framebufferHeight;
        glfwGetFramebufferSize(window, &framebufferWidth, &framebufferHeight);
        glViewport(0, 0, framebufferWidth, framebufferHeight);
    }

    void keyAction(GLFWwindow* window, int key, int scancode, int action, int mods) {
        if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
            glfwSetWindowShouldClose(window, 1);
        }
    }
}

namespace lifetime{
    uint8_t IMGUIAlive; //Is IMGUI initialized
    uint8_t GLFWAlive; //Is GLFW initialized
    uint8_t SPDLOGAlive; //Is SPDLOG initialized

    inline void initSPDLOG() {
        spdlog::set_level(spdlog::level::info);
        SPDLOGAlive = 1;
    }

    inline void initGLFW() {
        if(glfwInit() != GLFW_TRUE) {
            spdlog::error("GLFW couldn't init.");
            lifetime::killAll(1);
        }
        else {
            lifetime::GLFWAlive = 1;
        }
    }

    inline void initIMGUI(GLFWwindow* window) {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
        ImGui_ImplGlfw_InitForOpenGL(window, 1);
        ImGui_ImplOpenGL3_Init();
        IMGUIAlive = 1;
    }

    inline void killIMGUI() {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
        IMGUIAlive = 0;
    }

    /// @brief Kills all. Logs event with GLFW timestamp.
    /// @param error If set to 1, logs error & aborts. If 0, logs info & DOESN'T ABORT.
    inline void killAll(uint8_t error) {
        if(GLFWAlive) { // Log Event
            if(error) {
                spdlog::error("You've done it again... Committing dead. [glfwTime : {0:.3f}(s)]", glfwGetTime());
            } 
            else {
                spdlog::info("GLFW terminating. [glfwTime : {0:.3f}(s)]", glfwGetTime());
            }
        } 
        else {
            if(error) {
                spdlog::error("You've done it again... Committing dead. [glfwTime : NOGLFW]");
            } 
        }

        if(IMGUIAlive) {
            killIMGUI();
            IMGUIAlive = 0;
        }
        if(GLFWAlive) {
            glfwTerminate();
            GLFWAlive = 0;
        }

        if(error) {
            abort();
        }
    }
}