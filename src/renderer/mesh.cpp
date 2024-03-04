#include "renderer/mesh.hpp"

#include "spdlog/spdlog.h"

namespace renderer {
    d_Mesh* loadMeshFromStaticBuffer(std::string_view meshName, size_t vertexBufferSize, const GLfloat* vertexBuffer, size_t elementBufferSize, const GLuint* elementBuffer) {
        spdlog::info("Loading (static) mesh [{0}] with {1} buffer size.", meshName.data(), vertexBufferSize);

        d_Mesh* data = new d_Mesh;

        data->sizeOfVertexBuffer = vertexBufferSize;
        data->vertexBuffer = new unsigned char[vertexBufferSize];
        memcpy(data->vertexBuffer, vertexBuffer, vertexBufferSize);

        data->numElements = elementBufferSize / sizeof(GLuint);
        data->elementBuffer = new GLuint[elementBufferSize];
        memcpy(data->elementBuffer, elementBuffer, elementBufferSize);

        return data;
    }

    d_Mesh* loadMeshFromAssimpMesh(aiMesh* assimpMesh) {
        spdlog::info("Loading (assimp) mesh [{0}] with {1} vertices.", assimpMesh->mName.C_Str(), assimpMesh->mNumVertices);

        d_Mesh* data = new d_Mesh;

        data->sizeOfVertexBuffer = assimpMesh->mNumVertices * sizeof(Vertex);
        Vertex* vertexData = new Vertex[assimpMesh->mNumVertices];
        data->vertexBuffer = (unsigned char*)vertexData;

        int numVertices = assimpMesh->mNumVertices;
        for(size_t i = 0; i < numVertices; i++) {
            memcpy(&vertexData[i].pos, &assimpMesh->mVertices[i], sizeof(GLfloat) * 3);
            memcpy(&vertexData[i].normal, &assimpMesh->mNormals[i], sizeof(GLfloat) * 3);
            memcpy(&vertexData[i].tangent, &assimpMesh->mTangents[i], sizeof(GLfloat) * 3);
            memcpy(&vertexData[i].bitangent, &assimpMesh->mBitangents[i], sizeof(GLfloat) * 3);
            memcpy(&vertexData[i].UV, &assimpMesh->mTextureCoords[i], sizeof(GLfloat) * 2);
        }

        data->numElements = assimpMesh->mNumFaces * 3;
        data->elementBuffer = new GLuint[data->numElements];
        for(size_t i = 0; i < assimpMesh->mNumFaces; i++) {
            memcpy(&data->elementBuffer[i * 3], &assimpMesh->mFaces->mIndices, sizeof(GLuint) * 3);
        }

        return data;
    }
    
    void freeMesh(d_Mesh* data) {
        delete[] data->vertexBuffer;
        delete[] data->elementBuffer;

        delete data;
    }

    void pushMesh(Mesh& mesh, d_Mesh* data) {
        int numVertexAttributes = 5;
        int vertexAttributeSizes[] = { 3, 3, 3, 3, 2 };


        glGenBuffers(1, &mesh.VBOID);
        glBindBuffer(GL_ARRAY_BUFFER, mesh.VBOID);
        glBufferData(GL_ARRAY_BUFFER, data->sizeOfVertexBuffer, data->vertexBuffer, GL_STATIC_DRAW);


        glGenBuffers(1, &mesh.EBOID);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.EBOID);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, data->numElements * sizeof(GLuint), data->elementBuffer, GL_STATIC_DRAW);
        mesh.numElements = data->numElements;


        glGenVertexArrays(1, &mesh.VAOID);
        glBindVertexArray(mesh.VAOID);

        glBindBuffer(GL_ARRAY_BUFFER, mesh.VBOID);

        //calculate attribute stride
        size_t stride = 0; 
        for(size_t i = 0; i < numVertexAttributes; i++)
            stride += vertexAttributeSizes[i];

        //enable attributes
        size_t cursor = 0;
        for(size_t i = 0; i < numVertexAttributes; i++) {
            glVertexAttribPointer(i, vertexAttributeSizes[i], GL_FLOAT, GL_FALSE, stride * sizeof(GLfloat), (void*)(cursor * sizeof(GLfloat)));
            cursor += vertexAttributeSizes[i];
            glEnableVertexAttribArray(i);
        }

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.EBOID);
    }

    void useMesh(const Mesh& mesh) {
        glBindVertexArray(mesh.VAOID);
    }

    void drawMesh(const Mesh& mesh) {
        glDrawElements(GL_TRIANGLES, mesh.numElements, GL_UNSIGNED_INT, (void *)0);
    }
}