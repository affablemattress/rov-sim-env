#pragma once

#include "config.hpp"

#include "GLFW/glfw3.h"
#include "glad/glad.h"
#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include "spdlog/spdlog.h"

/**
 * @brief Manages GLFW, IMGUI & SPDLOG lifetimes.
 */
namespace lifetime {
    extern uint8_t IMGUIAlive;   /** Is IMGUI initialized */
    extern uint8_t GLFWAlive;    /** Is GLFW initialized */
    extern uint8_t SPDLOGAlive;  /** Is SPDLOG initialized */

    extern void initSPDLOG();
    extern void initGLFW();
    extern void initIMGUI(GLFWwindow* window);
    extern void killIMGUI();
    
    /**
     * @brief Kills all. Logs event with GLFW timestamp.
     * 
     * @param error If set to 1, logs error & aborts. If 0, logs info & DOESN'T ABORT.
     */
    extern void killAll(uint8_t error);
}