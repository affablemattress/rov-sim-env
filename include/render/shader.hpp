#pragma once

#include "config.hpp"

#include "glad/glad.h"

#include <unordered_map>
#include <string>


#define SHADER_SOURCE_BUFFER_SIZE 8192
#define SHADER_ERROR_BUFFER_SIZE 512

namespace shader {
    struct Program {
        GLuint id;
        std::unordered_map<std::string, GLint> uniforms;
    };

    void compileProgram(shader::Program& program, const char* vertexShaderPath, const char* fragmentShaderPath);

    void pushUniform(shader::Program& program, const GLchar* uniformName);
    void pushUniforms(shader::Program& program, size_t sizeOfNames, const GLchar** uniformNamesArray);

    template <typename F, typename... V>
    void setUniform(shader::Program& program, F glUniformFunction, const GLchar* uniformName, V... args) {
        glUniformFunction(program.uniforms[uniformName], args...);
    }

    void bindUniformBlock(shader::Program& program, const GLchar* blockName, GLint bindIndex);
}
