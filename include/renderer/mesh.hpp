#pragma once

#include "config.hpp"

#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "glad/glad.h"

#include <vector>

namespace renderer {
    #pragma pack(1)
    struct Vertex {
        glm::vec3 pos;
        glm::vec3 normal;
        glm::vec2 UV;
    };
    #pragma pack()

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
    };
}