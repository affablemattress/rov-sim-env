#include "renderer/part.hpp"

namespace renderer {
    Part::Part(math::TransformMatrix* localTransformMatrix, renderer::Mesh* mesh, 
               renderer::Texture2D* diffuseMap, renderer::Texture2D* specularMap, renderer::Texture2D* normalMap, 
               renderer::Shader* shader) : 
                   localTransformMatrix(localTransformMatrix), mesh(mesh), 
                   diffuseMap(diffuseMap), specularMap(specularMap), normalMap(normalMap),
                   shader(shader) {}

    void usePart(const renderer::Part& part) {
        renderer::useShader(*part.shader);

        renderer::useMesh(*part.mesh);

        renderer::useTexture2D(*part.diffuseMap, GL_TEXTURE0);
        renderer::useTexture2D(*part.specularMap, GL_TEXTURE0 + 1);
    }

    void drawPart(const renderer::Part& part) {
        glDrawElements(GL_TRIANGLES, part.mesh->numOfElements, GL_UNSIGNED_INT, (void *)0);
    }
}