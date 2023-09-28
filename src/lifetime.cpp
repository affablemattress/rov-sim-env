#include "lifetime.hpp"


namespace lifetime{
    uint8_t IMGUIAlive;
    uint8_t GLFWAlive;
    uint8_t SPDLOGAlive;

    void initSPDLOG() {
        spdlog::set_level(spdlog::level::info);
        SPDLOGAlive = 1;
    }

    void initGLFW() {
        if(glfwInit() != GLFW_TRUE) {
            spdlog::error("GLFW couldn't init.");
            lifetime::killAll(1);
        }
        else {
            lifetime::GLFWAlive = 1;
        }
    }

    void initIMGUI(GLFWwindow* window) {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
        ImGui_ImplGlfw_InitForOpenGL(window, 1);
        ImGui_ImplOpenGL3_Init();
        IMGUIAlive = 1;
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