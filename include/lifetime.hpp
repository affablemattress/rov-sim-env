#pragma once

#include "config.hpp"

#include "spdlog/spdlog.h"
#include "GLFW/glfw3.h"

/**
 * @brief Manages GLFW, IMGUI & SPDLOG lifetimes.
 */
namespace lifetime {
    extern uint8_t IMGUIAlive;   /** Is IMGUI initialized */
    extern uint8_t GLFWAlive;    /** Is GLFW initialized */
    extern uint8_t SPDLOGAlive;  /** Is SPDLOG initialized */
    extern uint8_t SPDLOGAlive;  /** Is SPDLOG initialized */
    extern uint8_t STBIAlive;   /** Is STBI initialized */
    extern uint8_t GLADAlive;    /** Is GLAD initialized */

    extern void initSPDLOG(spdlog::level::level_enum logLevel, std::string_view infoPropt);
    extern void initGLFW(GLFWerrorfun errorCallback, std::string_view infoPromt);
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