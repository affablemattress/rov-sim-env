#pragma once

#include "config.hpp"

#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "glad/glad.h"

#include <vector>

namespace renderer {
    struct VBO {
        GLuint bufferID;

        VBO() = delete;
        VBO(size_t sizeOfBuffer, const GLfloat* buffer);
    };

    struct EBO {
        GLuint bufferID;

        EBO() = delete;
        EBO(size_t sizeOfBuffer, const GLuint* buffer);
    };

    struct VAO {
        GLuint bufferID;

        VAO() = delete;
        VAO(VBO& vertexBuffer, EBO& elementBuffer, size_t numberOfAttributes, const size_t attributeSizeList[]);
    };

    
    struct Mesh {
        renderer::VBO VBO;
        renderer::EBO EBO;
        renderer::VAO VAO;

        size_t numOfElements;

        Mesh(size_t sizeOfVertexBuffer, const GLfloat* vertexBuffer, 
             size_t sizeOfElementBuffer, const GLuint* elementBuffer,
             size_t numberOfVertexAttributes, const size_t vertexAttributeSizeList[]);
    };

    void useMesh(const Mesh& vao);
}