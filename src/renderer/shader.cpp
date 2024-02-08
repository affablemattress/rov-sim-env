#include "renderer/shader.hpp"

#include "app.hpp"

#include "GLFW/glfw3.h"
#include "glad/glad.h"
#include "spdlog/spdlog.h"

#include <stdio.h>

namespace renderer{
    namespace shader {
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
            delete[] shaderSource;
            delete[] compilationInfoLog; 

            return shader;
        }

        void compileProgram(shader::Program& program, const char* vertexShaderPath, const char* fragmentShaderPath) {
            GLuint vertexShader = compileShaderFromPath(vertexShaderPath, GL_VERTEX_SHADER);
            glAttachShader(program.id, vertexShader);

            GLuint fragmentShader = compileShaderFromPath(fragmentShaderPath, GL_FRAGMENT_SHADER);
            glAttachShader(program.id, fragmentShader);

            glLinkProgram(program.id);
        }

        void pushUniform(shader::Program& program, const GLchar* uniformName) {
            program.uniforms[uniformName] = glGetUniformLocation(program.id, uniformName);
        }

        void pushUniforms(shader::Program& program, size_t sizeOfNames, const GLchar** uniformNamesArray) {
            for (size_t i = 0; i < sizeOfNames; i++){
                pushUniform(program, uniformNamesArray[i]);
            }
        }

        void bindUniformBlock(shader::Program& program, const GLchar* blockName, GLint bindIndex) {
            glUniformBlockBinding(program.id, glGetUniformBlockIndex(program.id, blockName), bindIndex);
        }
    }
}
