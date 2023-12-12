#pragma once

#include "config.hpp"

#include "glad/glad.h"

#include <unordered_map>
#include <string>


#define SHADER_SOURCE_BUFFER_SIZE 8192
#define SHADER_ERROR_BUFFER_SIZE 512

namespace shader {
    struct program {
        GLuint id;
        std::unordered_map<std::string, GLint> uniforms;
    };

    void compileProgram(shader::program* program, const char* vertexShaderPath, const char* fragmentShaderPath);

    void pushUniform(shader::program* program, const GLchar* uniformName);
    void pushUniforms(shader::program* program, size_t sizeOfNames, const GLchar** uniformNamesArray);
}
