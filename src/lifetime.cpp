#include "lifetime.hpp"

#include "glad/glad.h"
#include "imgui.h"
#include "stb_image.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include "spdlog/spdlog.h"

namespace lifetime{
    uint8_t IMGUIAlive = 0;
    uint8_t GLFWAlive = 0;
    uint8_t SPDLOGAlive = 0;
    uint8_t STBIAlive = 0;
    uint8_t GLADAlive = 0;

    void initSPDLOG(spdlog::level::level_enum logLevel, std::string_view infoPrompt) {
        spdlog::set_level(logLevel);
        spdlog::info(infoPrompt);
        SPDLOGAlive = 1;
    }

    void initSTBI() {
        stbi_set_flip_vertically_on_load(1);
        STBIAlive = 1;
    }

    void initGLFW(GLFWerrorfun errorCallback, std::string_view infoPrompt) {
        if(glfwInit() != GLFW_TRUE) {
            spdlog::error("GLFW couldn't init.");
            lifetime::killAll(1);
        }
        else {
            spdlog::info(infoPrompt);
            lifetime::GLFWAlive = 1;
        }
    }

    void initIMGUI(GLFWwindow* window) {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
        ImGui_ImplGlfw_InitForOpenGL(window, 1);
        ImGui_ImplOpenGL3_Init();
        ImGui::StyleColorsLight();
        IMGUIAlive = 1;
    }

    void initGLAD() {
        bool gladStatus = gladLoadGLLoader((GLADloadproc)&glfwGetProcAddress);
        if (!gladStatus) {
            spdlog::error("GLAD couldn't load GLProc's");
            lifetime::killAll(1);
        }
        else{
            GLADAlive = 1;
        }
    }

    void killIMGUI() {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
        IMGUIAlive = 0;
    }

    void killAll(uint8_t error) {
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