#include "renderer/shader.hpp"

#include "app.hpp"

#include "GLFW/glfw3.h"
#include "glad/glad.h"
#include "spdlog/spdlog.h"

#include <stdio.h>

// ! ||--------------------------------------------------------------------------------||
// ! ||                                 COMPILE SHADER                                 ||
// ! ||--------------------------------------------------------------------------------||

static GLuint compileShaderFromPath(const char* filepath, GLint shaderType) {
    spdlog::info("Compiling shader (type: {0}) at path: {1}", shaderType, filepath);
    FILE* shaderFile = fopen(filepath, "r");
    if(!shaderFile) {
        spdlog::error("Couldn't open file: {0}", filepath);
        app::lifetime::killAll(1);
    }
    GLchar* shaderSource = new GLchar[SHADER_SOURCE_BUFFER_SIZE];
    size_t sourceSize = fread(shaderSource, 1, SHADER_SOURCE_BUFFER_SIZE - 1, shaderFile);
    shaderSource[sourceSize] = '\0';

    GLuint shader = glCreateShader(shaderType);
    glShaderSource(shader, 1, &shaderSource, NULL);
    glCompileShader(shader);
    
    GLint  compilationStatus;
    GLchar* compilationInfoLog = new char[SHADER_ERROR_BUFFER_SIZE];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compilationStatus);
    if(!compilationStatus) {
        glGetShaderInfoLog(shader, SHADER_ERROR_BUFFER_SIZE - 1, NULL, compilationInfoLog);
        spdlog::error("GLSL compilation error.\n   >GLSL error description start\n\n{0}\n   >GLSL error description end", compilationInfoLog);
        app::lifetime::killAll(1);
    }

    fclose(shaderFile);
    delete[] compilationInfoLog; 
    delete[] shaderSource;

    return shader;
}

static void compileProgram(renderer::Shader& program, const char* vertexShaderPath, const char* fragmentShaderPath) {
    GLuint vertexShader = compileShaderFromPath(vertexShaderPath, GL_VERTEX_SHADER);
    glAttachShader(program.id, vertexShader);

    GLuint fragmentShader = compileShaderFromPath(fragmentShaderPath, GL_FRAGMENT_SHADER);
    glAttachShader(program.id, fragmentShader);

    glLinkProgram(program.id);
}

// ! ||--------------------------------------------------------------------------------||
// ! ||                         ADD UNIFORM IDS TO THE HASH MAP                        ||
// ! ||--------------------------------------------------------------------------------||

static void pushUniform(renderer::Shader& program, const GLchar* uniformName) {
    program.uniforms[uniformName] = glGetUniformLocation(program.id, uniformName);
}

static void pushUniforms(renderer::Shader& program, size_t sizeOfNames, const GLchar** uniformNamesArray) {
    for (size_t i = 0; i < sizeOfNames; i++){
        pushUniform(program, uniformNamesArray[i]);
    }
}

// ! ||--------------------------------------------------------------------------------||
// ! ||                        BIND UNIFORM BLOCKS OF THE SHADER                       ||
// ! ||--------------------------------------------------------------------------------||

static void bindUniformBlock(renderer::Shader& program, const GLchar* blockName, GLint bindIndex) {
    glUniformBlockBinding(program.id, glGetUniformBlockIndex(program.id, blockName), bindIndex);
}

static void bindUniformBlocks(renderer::Shader& program, size_t numOfUniformBlocks, const GLchar** uniformBlockNamesArray, const GLint* uniformBlockBindingPointsArray) {
    for(size_t i = 0; i < numOfUniformBlocks; i++) {
        bindUniformBlock(program, uniformBlockNamesArray[i], uniformBlockBindingPointsArray[i]);
    }
}

namespace renderer{
    Shader::Shader(std::string_view vertexShaderPath, std::string_view fragmentShaderPath,
                   size_t sizeOfUniformNames, const GLchar** uniformNamesArray, 
                   size_t numOfUniformBlocks, const GLchar** uniformBlockNamesArray, const GLint* uniformBlockBindingPointsArray) {
        id = glCreateProgram();
        compileProgram(*this, PROJECT_PATH "res/shader/vertexMain.glsl", PROJECT_PATH "res/shader/fragmentMain.glsl");
        pushUniforms(*this, sizeOfUniformNames, uniformNamesArray);
        bindUniformBlocks(*this, numOfUniformBlocks, uniformBlockNamesArray, uniformBlockBindingPointsArray);
    }

    void useShader(const Shader& shader) {
        glUseProgram(shader.id);
    }
}
