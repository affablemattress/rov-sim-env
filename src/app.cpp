#include "app.hpp"
#include "window/callbackGLFW.hpp"

#include "glad/glad.h"
#include "imgui.h"
#include "stb_image.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include "spdlog/spdlog.h"

namespace app {
    const Settings settings = {
        .defaultWindowWidth = 800,
        .defaultWindowHeight = 600,
        
        .mouseSensitivity = 0.1,
        .movementSpeed = 1.0,
        .sprintModifier = 1.5,

        .cameraMaxY = 85.,
        .cameraMinY = -85.
    };

    WindowVars window_vars = {
        .framebufferWidth = 1,
        .framebufferHeight = 1,

        .frametime = 0.0166
    };


    StateVars state_vars = {
        .isMouseEnabled = 1,
        .isWireframe = 0
    };

    namespace lifetime {
        struct States {
            uint8_t IMGUIAlive;
            uint8_t GLFWAlive;
            uint8_t SPDLOGAlive;
            uint8_t STBIAlive;
            uint8_t GLADAlive;
        };

        static States* states = nullptr;
        void startApp() {
            srand(time(NULL));

            states = new States();
            memset(states, 0, sizeof(States));
        }

        void endApp() {
            delete states;
            states = nullptr;
        }

        void initSPDLOG(spdlog::level::level_enum logLevel, std::string_view infoPrompt) {
            spdlog::set_level(logLevel);
            spdlog::info(infoPrompt);
            states->SPDLOGAlive = 1;
        }
        void initSTBI() {
            stbi_set_flip_vertically_on_load(1);
            states->STBIAlive = 1;
        }

        void initGLFW(GLFWerrorfun errorCallback, std::string_view infoPrompt) {
            if(glfwInit() != GLFW_TRUE) {
                spdlog::error("GLFW couldn't init.");
                lifetime::killAll(1);
            }
            else {
                spdlog::info(infoPrompt);
                glfwSetErrorCallback((GLFWerrorfun)&callbackGLFW::error);
                states->GLFWAlive = 1;
            }
        }

        void initIMGUI(GLFWwindow* window) {
            IMGUI_CHECKVERSION();
            ImGui::CreateContext();
            ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
            ImGui_ImplGlfw_InitForOpenGL(window, 1);
            ImGui_ImplOpenGL3_Init();
            ImGui::StyleColorsLight();
            states->IMGUIAlive = 1;
        }

        void initGLAD() {
            bool gladStatus = gladLoadGLLoader((GLADloadproc)&glfwGetProcAddress);
            if (!gladStatus) {
                spdlog::error("GLAD couldn't load GLProc's");
                lifetime::killAll(1);
            }
            else{
                states->GLADAlive = 1;
            }
        }

        void killIMGUI() {
            ImGui_ImplOpenGL3_Shutdown();
            ImGui_ImplGlfw_Shutdown();
            ImGui::DestroyContext();
            states->IMGUIAlive = 0;
        }

        void killAll(uint8_t error) {
            if(states->GLFWAlive) { // Log Event
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

            if(states->IMGUIAlive) {
                killIMGUI();
                states->IMGUIAlive = 0;
            }
            if(states->GLFWAlive) {
                glfwTerminate();
                states->GLFWAlive = 0;
            }

            if(error) {
                abort();
            }
        }
    }
}