#pragma once

#include "config.hpp"

#include "glad/glad.h"

#include <unordered_map>
#include <string>


#define SHADER_SOURCE_BUFFER_SIZE 8192
#define SHADER_ERROR_BUFFER_SIZE 512

namespace renderer{
    struct Shader {
        GLuint id;
        std::unordered_map<std::string, GLint> uniforms;

        Shader(std::string_view vertexShaderPath, std::string_view fragmentShaderPath, 
                size_t sizeOfUniformNames, const GLchar** uniformNamesArray, 
                size_t numOfUniformBlocks, const GLchar** uniformBlockNamesArray, const GLint* uniformBlockBindingPointsArray);
    };

    void useShader(const Shader& shader);

    template <typename F, typename... V>
    void setUniform(Shader& shader, F glUniformFunction, const GLchar* uniformName, V... args) {
        glUniformFunction(shader.uniforms[uniformName], args...);
    }
}
