#pragma once

#include "config.hpp"

#include "spdlog/spdlog.h"
#include "GLFW/glfw3.h"

namespace app {
    struct Settings {
        int16_t defaultWindowWidth;
        int16_t defaultWindowHeight;

        double mouseSensitivity;
        double movementSpeed;

        double cameraMaxY;
        double cameraMinY;
    };
    extern const Settings settings;

//--------------------------------------------------------------------------------

    struct WindowVars {
        int16_t framebufferWidth;
        int16_t framebufferHeight;

        double frametime;
    };
    extern WindowVars window_vars;

//--------------------------------------------------------------------------------

    struct StateVars {
        uint8_t isMouseEnabled;
        bool isWireframe;
    };
    extern StateVars state_vars;

//--------------------------------------------------------------------------------

    namespace lifetime {
        extern void startApp();
        extern void endApp();

        extern void initSPDLOG(spdlog::level::level_enum logLevel, std::string_view infoPrompt);
        extern void initGLFW(GLFWerrorfun errorCallback, std::string_view infoPrompt);
        extern void initIMGUI(GLFWwindow* window);
        extern void initSTBI();
        extern void initGLAD();
        extern void killIMGUI();

        /**
         * @brief Kills all. Logs event with GLFW timestamp.
         * 
         * @param error If set to 1, logs error & aborts. If 0, logs info & DOESN'T ABORT.
         */
        extern void killAll(uint8_t error);
    }
}