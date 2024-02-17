#include "renderer/mesh.hpp"

namespace renderer {
    VBO::VBO(size_t sizeOfBuffer, const GLfloat* buffer) {
        glGenBuffers(1, &bufferID);
        glBindBuffer(GL_ARRAY_BUFFER, bufferID);
        glBufferData(GL_ARRAY_BUFFER, sizeOfBuffer, buffer, GL_STATIC_DRAW);
    }

    EBO::EBO(size_t sizeOfBuffer, const GLuint* buffer) {
        glGenBuffers(1, &bufferID);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferID);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeOfBuffer, buffer, GL_STATIC_DRAW);
    }

    VAO::VAO(VBO& vertexBuffer, EBO& elementBuffer, size_t numberOfAttributes, const size_t attributeSizeList[]) {
        glGenVertexArrays(1, &bufferID);
        glBindVertexArray(bufferID);

        glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer.bufferID);
        size_t stride = 0;
        for(size_t i = 0; i < numberOfAttributes; i++) {
            stride += attributeSizeList[i];
        }
        size_t attributeCursor = 0;
        for(size_t i = 0; i < numberOfAttributes; i++) {
            glVertexAttribPointer(i, attributeSizeList[i], GL_FLOAT, GL_FALSE, stride * sizeof(GLfloat), (void*)(attributeCursor * sizeof(GLfloat)));
            attributeCursor += attributeSizeList[i];
            glEnableVertexAttribArray(i);
        }

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer.bufferID);
    }

    Mesh::Mesh(size_t sizeOfVertexBuffer, const GLfloat* vertexBuffer,
               size_t sizeOfElementBuffer, const GLuint* elementBuffer,
               size_t numberOfVertexAttributes, const size_t vertexAttributeSizeList[]) : 
               VBO(sizeOfVertexBuffer, vertexBuffer), 
               EBO(sizeOfElementBuffer, elementBuffer),
               VAO(this->VBO, this->EBO, numberOfVertexAttributes, vertexAttributeSizeList) {
                   numOfElements = sizeOfElementBuffer;
               }

    void useMesh(const Mesh& mesh) {
        glBindVertexArray(mesh.VAO.bufferID);
    }
}