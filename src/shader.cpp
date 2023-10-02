#include "shader.hpp"

namespace shader {
    GLuint compileShaderFromPath(const char* filepath, GLint shaderType) {
        spdlog::info("Compiling (type: {0}) shader at path: {1}", shaderType, filepath);
        FILE* shaderFile = fopen(filepath, "r");
        if(!shaderFile) {
            spdlog::error("Couldn't open file: {0}", filepath);
            lifetime::killAll(1);
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
            lifetime::killAll(1);
        }

        fclose(shaderFile);
        delete[] shaderSource;
        delete[] compilationInfoLog; 

        return shader;
    }
}