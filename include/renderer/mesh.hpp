#pragma once

#include "config.hpp"

#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glad/glad.h"
#include "assimp/postprocess.h"
#include "assimp/scene.h"
#include "assimp/DefaultLogger.hpp"

#include <string_view>
#include <vector>

namespace renderer {
    #pragma pack(1)
    struct Vertex {
        GLfloat pos[3];
        GLfloat normal[3];
        GLfloat tangent[3];
        GLfloat bitangent[3];
        GLfloat UV[2];
    };
    #pragma pack()

    struct d_Mesh {
        int sizeOfVertexBuffer;
        unsigned char* vertexBuffer;

        int numElements;
        GLuint* elementBuffer;
    };
    
    struct Mesh {
        GLuint VBOID;
        GLuint EBOID;
        GLuint VAOID;

        int numElements;
    };

    d_Mesh* loadMeshFromStaticBuffer(std::string_view meshName, size_t vertexBufferSize, const GLfloat* vertexBuffer, size_t elementBufferSize, const GLuint* elementBuffer);
    d_Mesh* loadMeshFromAssimpMesh(aiMesh* assimpMesh);
    void freeMesh(d_Mesh* data);

    void pushMesh(Mesh& mesh, d_Mesh* data);

    void useMesh(const Mesh& mesh);

    void drawMesh(const Mesh& mesh);
}