#pragma once

#include "config.hpp"
#include "lifetime.hpp"

#include "GLFW/glfw3.h"
#include "glad/glad.h"
#include "spdlog/spdlog.h"

#include <stdio.h>

#define SHADER_SOURCE_BUFFER_SIZE 8192
#define SHADER_ERROR_BUFFER_SIZE 512

namespace shader {
    /**
     * @brief Attempts to read and compile the shader at 'filepath'. Size of the source buffer is SHADER_SOURCE_BUFFER_SIZE.
     * 
     * @param filepath absoulute path to the .glsl file.
     * @param shaderType type of the shader (GL_XXX_SHADER)
     * @return GLuint shader handle.
     */
    GLuint compileShaderFromPath(const char* filepath, GLint shaderType);
}